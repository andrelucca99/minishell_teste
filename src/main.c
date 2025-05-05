/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andre <andre@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:07:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/05 14:28:42 by andre            ###   ########.fr       */
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
            char *outfile = NULL;
            int out_fd = -1;
            int redir = detect_output_redir(args, &outfile);

            if (redir)
            {
                out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_fd < 0)
                {
                    perror("open");
                    free_args(args);
                    free(line);
                    continue;
                }
                int saved_stdout = dup(STDOUT_FILENO);
                dup2(out_fd, STDOUT_FILENO);

                if (is_builtin(args[0]))
                    exec_builtin(args);
                else
                    execute_command(args);

                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
                close(out_fd);
            }
            else
            {
                if (is_builtin(args[0]))
                    exec_builtin(args);
                else
                    execute_command(args);
            }
        }
        free_args(args);
        free(line);
    }
    return (0);
}
