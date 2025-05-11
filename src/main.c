/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andre <andre@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:56:29 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/11 14:16:40 by andre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	char *line;
	t_token *tokens;
	t_command *cmds;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		cmds = parse_tokens(tokens);
		execute_commands(cmds);

		free_commands(cmds);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
