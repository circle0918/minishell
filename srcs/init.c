/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 12:56:01 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/16 19:57:23 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_global_struct(t_ms *g)
{
	g->ret_errno = 0;
	g->cmd_tab = NULL;
	g->exit = 0;
	g->line = NULL;
	g->env = NULL;
	g->path = NULL;
	g->cmd = NULL;
	g->cmd_tmp = NULL;
	g->error = NULL;
//	g->return_code = 0;
}
