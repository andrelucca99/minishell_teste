/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:30:34 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/14 19:38:59 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	should_execute_builtin_in_parent(t_command *cmd)
{
	return (!cmd->next && is_builtin(cmd->args[0])
		&& !cmd->input_file && !cmd->output_file);
}
