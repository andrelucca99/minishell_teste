/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/07 14:39:06 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_executable(char *cmd)
{
	char	*path;
	char	*paths;
	char	*token;
	char	fullpath[1024];

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_strdup(path);
	token = strtok(paths, ":");
	while (token)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);
		if (access(fullpath, X_OK) == 0)
		{
			free(paths);
			return (ft_strdup(fullpath));
		}
		token = strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

char	***split_pipeline(char **args, int *num_cmds)
{
	int		i = 0;
	int		j = 0;
	int		k = 0;
	char	***commands = malloc(sizeof(char **) * MAX_ARGS);

	commands[k] = malloc(sizeof(char *) * MAX_ARGS);
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			commands[k][j] = NULL;
			k++;
			j = 0;
			commands[k] = malloc(sizeof(char *) * MAX_ARGS);
		}
		else
			commands[k][j++] = ft_strdup(args[i]);
		i++;
	}
	commands[k][j] = NULL;
	*num_cmds = k + 1;
	return (commands);
}

void	execute_pipeline(char ***cmds, int n)
{
	int		pipefd[2];
	int		in_fd;
	int		i;
	pid_t	pid;
	char	*path;

	in_fd = 0;
	i = 0;
	while (i < n)
	{
		pipe(pipefd);
		if ((pid = fork()) == 0)
		{
			dup2(in_fd, 0);
			if (i < n - 1)
				dup2(pipefd[1], 1);
			close(pipefd[0]);
			if (is_builtin(cmds[i][0]))
				exit(exec_builtin(cmds[i]));
			path = find_executable(cmds[i][0]);
			if (!path)
			{
				fprintf(stderr,
					"minishell: command not found: %s\n", cmds[i][0]);
				exit(127);
			}
			execve(path, cmds[i], environ);
			perror("execve");
			exit(1);
		}
		else
		{
			waitpid(pid, NULL, 0);
			close(pipefd[1]);
			in_fd = pipefd[0];
		}
		i++;
	}

}
