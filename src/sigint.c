/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andre <andre@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:46:15 by andre             #+#    #+#             */
/*   Updated: 2025/05/12 16:46:27 by andre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void handle_sigint(int signo)
{
  (void)signo;
  write(STDOUT_FILENO, "\n", 1);
  rl_on_new_line();
  rl_replace_line("", 0);
  rl_redisplay();
}