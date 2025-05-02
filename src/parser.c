/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/02 15:37:34 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*strip_quotes(char *str)
// {
// 	char	*clean;
// 	int		i;
// 	int		j;

// 	clean = malloc(ft_strlen(str) + 1);
// 	i = 0;
// 	j = 0;
// 	if (!clean)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] != '"' && str[i] != '\'')
// 			clean[j++] = str[i];
// 		i++;
// 	}
// 	clean[j] = '\0';
// 	return (clean);
// }

// char	**parse_input(char *line)
// {
// 	char	**args;
// 	char	*token;
// 	int		i;

// 	args = malloc(sizeof(char *) * MAX_ARGS);
// 	i = 0;

// 	if (!args)
// 		return (NULL);

// 	token = strtok(line, " ");
// 	while (token && i < MAX_ARGS - 1)
// 	{
// 		args[i++] = strip_quotes(token);
// 		token = strtok(NULL, " ");
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

char	*find_executable(char *cmd)
{
	char	*path;
	char	*paths;
	char	*token;

	if (strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_strdup(path);
	token = strtok(paths, ":");
	while (token)
	{
		char	fullpath[1024];

		snprintf(fullpath, sizeof(fullpath), "%s%s", token, cmd);
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

// void	execute_command(char **args)
// {
// 	pid_t	pid;
// 	int		status;
// 	char	*cmd_path;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		cmd_path = find_executable(args[0]);
// 		if (!cmd_path)
// 		{
// 			fprintf(stderr, "minishell: command not found: %s\n", args[0]);
// 			exit(127);
// 		}
// 		execve(cmd_path, args, environ);
// 		perror("minishell");
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pid > 0)
// 		waitpid(pid, &status, 0);
// 	else
// 		perror("fork");
// }
