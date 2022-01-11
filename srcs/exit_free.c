/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:33:33 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/13 12:42:39 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **split)
{
	int i;

	i = -1;
	if (split)
	{
		while (split[++i])
				free(split[i]);
		free(split);
	}
}

void	ft_exit(int nb, t_ms *g)
{
	if (nb == 2)
		ft_putstr("exit\n");
	ft_del_line(g->line);
	ft_lstclear(&g->env, &ft_del_list);
	ft_lstclear(&g->cmd, &ft_del_list);
	ft_lstclear(&g->cmd_tmp, &ft_del_list);
	ft_lstclear(&g->error, &ft_del_list);
	free_split(g->path);
	exit(1);
}
