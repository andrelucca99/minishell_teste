/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:49:54 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/14 18:52:01 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_commands(t_command *cmds)
{
	t_command	*tmp;
	int			i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
			{
				free(cmds->args[i]);
				i++;
			}
			free(cmds->args);
		}
		if (cmds->input_file)
			free(cmds->input_file);
		if (cmds->output_file)
			free(cmds->output_file);
		if (cmds->heredoc_delim)
			free(cmds->heredoc_delim);
		free(cmds);
		cmds = tmp;
	}
}

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **head, t_command *new)
{
	t_command	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}
