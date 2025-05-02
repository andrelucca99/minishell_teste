/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:25:57 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/02 15:52:03 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	return (strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "echo") == 0
		|| strcmp(cmd, "env") == 0
		|| strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0
		|| strcmp(cmd, "exit") == 0
	);
}
