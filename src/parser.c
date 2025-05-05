/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andre <andre@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/05 14:27:13 by andre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *find_executable(char *cmd)
{
	char *path;
	char *paths;
	char *token;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_strdup(path);
	token = strtok(paths, ":");
	while (token)
	{
		char fullpath[1024];

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
