/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:00:33 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 11:19:45 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		parsing_pipe(int i, int res, t_ms *g)
{
	int	tmp;

	tmp = 0;
	if (g->line[0] == '|' && g->line[1] == '|')
		return (parse_error(1, "||"));
	if (g->line[0] == '|')
		return (parse_error(1, "|"));
	if (g->line[i + 1] == '|' && g->line[i + 2] == '|' && g->line[i + 3] == '|')
		return (parse_error(1, "||"));
	if (g->line[i + 1] == '|' && g->line[i + 2] == '|')
		return (parse_error(1, "|"));
	res = i - 1;
	while (g->line[res] == ' ' && res > 0)
		res--;
	if (res == 0 && g->line[0] == ' ')
	{
		while (g->line[tmp] && g->line[tmp] == ' ')
		{
			if (g->line[tmp] == '|' && g->line[tmp + 1] == '|')
				return (parse_error(1, "||"));
			tmp++;
		}
		return (parse_error(1, "|"));
	}
	res = i + 1;
	while (g->line[res] && g->line[res] == ' ')
		res++;
	if (g->line[res] == '\0')
		return (parse_error(1, "newline"));
	if (g->line[res] == '|')
	{
		if (g->line[res + 1] == '|')
			return (parse_error(1, "||"));
		return (parse_error(1, "|"));
	}
	if (g->line[res] == '!')
		return (parse_error(1, "!"));
	if (g->line[res] == ';')
		return (parse_error(1, ";"));
	return (0);
}