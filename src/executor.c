/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:37:17 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/09 17:11:06 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// executor

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
	token = ft_strtok(paths, ":");
	while (token)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);
		if (access(fullpath, X_OK) == 0)
		{
			free(paths);
			return (ft_strdup(fullpath));
		}
		token = ft_strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

void	execute_commands(t_command *cmds)
{
	int		fd[2];
	int		in_fd;
	int		in;
	int		out;
	char	*path;
	pid_t	pid;

	in_fd = STDIN_FILENO;

	if (!cmds->next
		&& is_builtin(cmds->args[0])
		&& !cmds->input_file && !cmds->output_file)
	{
		exec_builtin(cmds->args);
		return ;
	}

	while (cmds)
	{
		if (cmds->next)
			pipe(fd);

		pid = fork();
		if (pid == 0)
		{
			if (cmds->input_file)
			{
				in = open(cmds->input_file, O_RDONLY);
				if (in < 0)
				{
					perror("open input");
					exit(1);
				}
				dup2(in, STDIN_FILENO);
				close(in);
			}
			else if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}

			if (cmds->output_file)
			{
				out = open(cmds->output_file,
					O_WRONLY | O_CREAT | (cmds->append_mode ? O_APPEND : O_TRUNC), 0644); // mudar a logica aqui!!!!
				if (out < 0)
				{
					perror("open output");
					exit(1);
				}
				dup2(out, STDOUT_FILENO);
				close(out);
			}
			else if (cmds->next)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}

			if (is_builtin(cmds->args[0]))
				exit(exec_builtin(cmds->args));

			path = find_executable(cmds->args[0]);
			if (!path)
			{
				fprintf(stderr, "command not found: %s\n", cmds->args[0]);
				exit(127);
			}
			execve(path, cmds->args, environ);
			perror("execve error");
			exit(1);
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmds->next)
		{
			close(fd[1]);
			in_fd = fd[0];
		}
		cmds = cmds->next;
	}
	while (wait(NULL) > 0);
}
