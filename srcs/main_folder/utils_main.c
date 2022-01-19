/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:56:21 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 18:57:04 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	lauch_all(t_ms *g)
{
	add_history(g->line);
	clean_command(g);
}

void	print_error_main(t_ms *g)
{
	print_list(g->error);
	ft_lstclear(&g->error, &ft_del_list);
}
