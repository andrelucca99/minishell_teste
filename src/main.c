/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:07:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/07 15:48:52 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <fcntl.h>

// Remove aspas simples e duplas de uma string
char *strip_quotes(char *str)
{
    char *clean = malloc(strlen(str) + 1);
    int i = 0, j = 0;
    if (!clean)
        return NULL;

    while (str[i])
    {
        if (str[i] != '\"' && str[i] != '\'')
            clean[j++] = str[i];
        i++;
    }
    clean[j] = '\0';
    return clean;
}

// Função para dividir a linha em argumentos, já limpando aspas simples/duplas
char **parse_input(char *line)
{
    char **args = malloc(sizeof(char *) * MAX_ARGS);
    int i = 0;

    if (!args)
        return NULL;

    char *token = strtok(line, " ");
    while (token && i < MAX_ARGS - 1)
    {
        args[i++] = strip_quotes(token);
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

int detect_output_redir(char **args, char **outfile)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
        {
            *outfile = args[i + 1];
            args[i] = NULL; // cortar a linha de argumentos aqui
            return (1);
        }
        i++;
    }
    return (0);
}

int detect_input_redir(char **args, char **infile)
{
    int i;
    int j;

    i = 0;
    while (args[i])
    {
        if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
        {
            *infile = ft_strdup(args[i + 1]);
			j = i;
			while (args[j + 2])
			{
				args[j] = args[j + 2];
				j++;
			}
            args[i] = NULL;
            return (1);
        }
        i++;
    }
    return (0);
}

void execute_command(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path = find_executable(args[0]);
    if (!cmd_path)
    {
        fprintf(stderr, "minishell: command not found: %s\n", args[0]);
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        execve(cmd_path, args, environ);
        perror("minishell");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
        perror("fork");
    free(cmd_path);
}

void	free_pipeline(char ***cmds, int n)
{
	int	i;
	int	j;

	i = 0;
	while (i < n)
	{
		j = 0;
		while (cmds[i][j])
		{
			free(cmds[i][j]);
			j++;
		}
		free(cmds[i]);
		i++;
	}
}

void free_args(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

void print_tokens(t_token *tokens) {
    while (tokens) {
        printf("Token: %-10s | Type: %d\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

int main(void)
{
    char *line;
    char **args;

    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);

        args = parse_input(line);
        if (args && args[0])
        {
			int		num_cmds = 0;
			char	***cmds = split_pipeline(args, &num_cmds);

			if (num_cmds == 1)
			{
				char *outfile = NULL, *infile = NULL;
				int out_fd = -1, in_fd = -1;
				int saved_stdout = -1, saved_stdin = -1;
				int redir_out = detect_output_redir(args, &outfile);
				int redir_in = detect_input_redir(args, &infile);

				if (redir_out)
				{
					out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (out_fd < 0)
					{
						perror("open");
						free_args(args);
						free(line);
						continue ;
					}
					saved_stdout = dup(STDOUT_FILENO);
					dup2(out_fd, STDOUT_FILENO);
				}
				if (redir_in)
				{
					in_fd = open(infile, O_RDONLY);
					if (in_fd < 0)
					{
						perror("open");
						free_args(args);
						free(line);
						continue ;
					}
					saved_stdin = dup(STDIN_FILENO);
					dup2(in_fd, STDIN_FILENO);
				}

				if (is_builtin(args[0]))
					exec_builtin(args);
				else
				{
					pid_t	pid = fork();
					if (pid == 0)
					{
						char	*path = find_executable(cmds[0][0]);
						if (!path)
						{
							fprintf(stderr, "minishell: command not found: %s\n", cmds[0][0]);
							exit(127);
						}
						execve(path, cmds[0], environ);
						perror("execve");
						exit(1);
					}
					else if (pid > 0)
						waitpid(pid, NULL, 0);
					else
						perror("fork");
				}
				// Restaura saída e entrada padrão
				if (redir_out)
				{
					dup2(saved_stdout, STDOUT_FILENO);
					close(saved_stdout);
					close(out_fd);
				}

				if (redir_in)
				{
					dup2(saved_stdin, STDIN_FILENO);
					close(saved_stdin);
					close(in_fd);
				}
			}
			else
				execute_pipeline(cmds, num_cmds);

			free_pipeline(cmds, num_cmds);
        }
        free_args(args);
        free(line);
    }
    return (0);
}
