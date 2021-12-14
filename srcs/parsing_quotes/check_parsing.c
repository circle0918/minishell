/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:07:49 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 10:59:35 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		parseur_quotes(t_ms *g, int i, int c)
{
	while (g->line[i] != c && g->line[i])
	{
		if (g->line[i] == '\\' && c != '\'')
			i++;
		i++;
	}
	if (g->line[i] == '\0')
		return (-1);
	return (i);
}
