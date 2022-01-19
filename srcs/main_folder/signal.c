/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:48:12 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 18:49:59 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end(int sig)
{
	(void)sig;
	if (g_ms->pid[0] != 0 && g_ms->pid[1] == 0)
	{
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
		g_ms->ret_errno = 131;
	}
	else
	{
		ft_putstr("\b \b\b \b");
		g_ms->ret_errno = 0;
	}
}

void	recovery(int sig)
{
	(void)sig;
	if (g_ms->pid[0] == 0 && g_ms->pid[1] == 0)
	{
		ft_putstr("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_ms->ret_errno = 130;
	}
	else
	{
		if (g_ms->pid[1] != 0)
			kill(g_ms->pid[1], SIGINT);
		else
			kill(g_ms->pid[0], SIGINT);
		write(1, "\n", 1);
		g_ms->ret_errno = 130;
	}
}
