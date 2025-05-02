/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:37:17 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/02 15:51:48 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_cd(char **args)
{
	if (!args[1])
		return (chdir(getenv("HOME")));
	if (chdir(args[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	exec_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	exec_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;

	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	exec_env(void)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	exec_export(char **args)
{
	char	*eq;
	int		i;

	i = 1;
	while (args[i])
	{
		eq = strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			setenv(args[i], eq + 1, 1);
		}
		else
			fprintf(stderr, "export: invalid format: %s\n", args[i]);
		i++;
	}
	return (0);
}

int	exec_unset(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unsetenv(args[i]);
		i++;
	}
	return (0);
}

int	exec_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1])
		status = ft_atoi(args[1]);
	printf("exit\n");
	exit(status);
}

int	exec_builtin(char **args)
{
	if (strcmp(args[0], "cd") == 0)
		return (exec_cd(args));
	else if (strcmp(args[0], "pwd") == 0)
		return (exec_pwd());
	else if (strcmp(args[0], "echo") == 0)
		return (exec_echo(args));
	else if (strcmp(args[0], "env") == 0)
		return (exec_env());
	else if (strcmp(args[0], "expot") == 0)
		return (exec_export(args));
	else if (strcmp(args[0], "unset") == 0)
		return (exec_unset(args));
	else if (strcmp(args[0], "exit") == 0)
		return (exec_exit(args));
	return (1);
}
