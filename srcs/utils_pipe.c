/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 14:35:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 17:49:43 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_pipe(t_ms *g)
{
	g->last_cmd = 0;
	g->nb_cmd_pipe = 0;
	g->pid[0] = 0;
	g->pid[1] = 0;
	g->ret_dir = NULL;
}

int		move_space_after(char *str, int i)
{
	while (str[i + 1] == ' ')
		i++;
	return (i);
}

int		move_space_before(char *str, int c)
{
	while (str[c - 1] == ' ')
		c--;
	return (c);
}

void	my_pipe(char **cmd, t_ms *g)
{
	int		fd_in[2];
	int		fd_out[2];
	int		status;

	status = 0;
	fd_in[0] = -1;
	fd_in[1] = -1;
	fd_out[0] = -1;
	fd_out[1] = -1;
	preexecution(cmd, fd_in, fd_out, g);
	while (waitpid(0, &status, 0) > 0)
	{
	}
	g->ret_errno = status / 256;
}
