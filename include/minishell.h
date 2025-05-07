/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:08:13 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/07 15:27:53 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define MAX_ARGS 128
# define MAX_CMDS 16

extern char	**environ;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

t_token	*tokenize(char *input);
void	free_tokens(t_token *tokens);

int		is_builtin(char *cmd);
int		exec_builtin(char **args);
void	execute_command(char **args);
char	**parse_input(char *line);
void	execute_command(char **args);
char	*find_executable(char *cmd);
char	***split_pipeline(char **args, int *num_cmds);
void	execute_pipeline(char ***cmds, int n);
int		detect_output_redir(char **args, char **outfile);
int		detect_input_redir(char **args, char **infile);

#endif
