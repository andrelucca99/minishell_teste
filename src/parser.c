/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/09 17:12:22 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// --- PARSER --- //

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = calloc(1, sizeof(t_command));
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

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmds;
	t_command	*cur;
	int			argc;
	int			i;
	char		*argv[MAX_ARGS] = {0};

	cmds = NULL;
	cur = new_command();
	argc = 0;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
			argv[argc++] = ft_strdup(tokens->value);
		else if (tokens->type == TOKEN_REDIR_IN && tokens->next)
		{
			tokens = tokens->next;
			cur->input_file = ft_strdup(tokens->value);
		}
		else if (tokens->type == TOKEN_REDIR_OUT && tokens->next)
		{
			tokens = tokens->next;
			cur->output_file = ft_strdup(tokens->value);
		}
		else if (tokens->type == TOKEN_REDIR_APPEND && tokens->next)
		{
			tokens = tokens->next;
			cur->output_file = ft_strdup(tokens->value);
			cur->append_mode = 1;
		}
		else if (tokens->type == TOKEN_HEREDOC && tokens->next)
		{
			tokens = tokens->next;
			cur->heredoc_delim = ft_strdup(tokens->value);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			argv[argc] = NULL;
			cur->args = calloc(argc + 1, sizeof(char *));
			i = 0;
			while (i < argc)
			{
				cur->args[i] = argv[i];
				i++;
			}
			add_command(&cmds, cur);
			cur = new_command();
			argc = 0;
		}
		tokens = tokens->next;
	}

	if (argc > 0)
	{
		argv[argc] = NULL;
		cur->args = calloc(argc + 1, sizeof(char *));
		i = 0;
		while (i < argc)
		{
			cur->args[i] = argv[i];
			i++;
		}
	}
	add_command(&cmds, cur);
	return (cmds);
}

t_token	*lexer(const char *line)
{
	t_token *tokens = NULL;
	int i = 0;
	while (line[i])
	{
		if (isspace(line[i]))
		{
			i++;
			continue;
		}

		int op_len = 0;
		t_token_type type = get_operator_type(&line[i], &op_len);

		if (type != TOKEN_WORD)
		{
			add_token(&tokens, new_token(type, ft_strndup(&line[i], op_len)));
			i += op_len;
		}
		else
		{
			int start = i;
			while (line[i] && !isspace(line[i]) && !ft_strchr("|<>", line[i]))
				i++;
			add_token(&tokens, new_token(TOKEN_WORD, ft_strndup(&line[start], i - start)));
		}
	}
	return (tokens);
}
