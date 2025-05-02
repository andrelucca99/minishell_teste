/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:07:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/02 15:16:29 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Remove aspas simples e duplas de uma string
char *strip_quotes(char *str)
{
    char *clean = malloc(strlen(str) + 1);
    int i = 0, j = 0;
    if (!clean) return NULL;

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

// Executa o comando via fork/execve
void execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("minishell");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
    }
    else
    {
        perror("fork");
    }
}

void free_args(char **args)
{
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
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
			if (is_builtin(args[0]))
				exec_builtin(args);
			else
            	execute_command(args);
		}
        free_args(args);
        free(line);
    }
    return (0);
}
