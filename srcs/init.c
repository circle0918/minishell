/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 12:56:01 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 18:35:20 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_global_struct(t_ms *g)
{
	g->ret = 1;
	g->ret_errno = 0;
	g->cmd_tab = NULL;
	g->exit = 0;
	g->line = NULL;
	g->env = NULL;
	g->path = NULL;
	g->cmd = NULL;
	g->cmd_tmp = NULL;
	g->error = NULL;
	g->unset_path = 0;
	g->redir_out_fd = 0;
	g->redir_in_fd = 0;
}
