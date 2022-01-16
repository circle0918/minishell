/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:47:18 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/16 23:51:44 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	norm_1(int res, t_ms *g, int comp)
{
	if (comp == 1)
	{
		if (g->line[res + 1] == '&')
			return (parse_error(1, "<&", g));
		if (g->line[res + 1] == '|')
			return (parse_error(1, ">|", g));
		if (g->line[res + 1] == '>')
			return (parse_error(1, ">>", g));
		return (parse_error(1, ">", g));
	}
	if (g->line[res + 1] == '&')
		return (parse_error(1, ">&", g));
	if (g->line[res + 1] == '>')
		return (parse_error(1, "<>", g));
	if (g->line[res + 1] == '<')
		return (parse_error(1, "<<", g));
	return (parse_error(1, "<", g));
}

int parsing_redirection_out(int i, int res, t_ms *g)
{
	int y;
	(void)res;

	y = 0;
	while (g->line[i + y] && g->line[i + y] == '>')
		y++;
	if (y == 3)
		return (parse_error(1, ">", g));
	else if (y > 3)
		return (parse_error(1, ">>", g));
	i += y;
	while (g->line[i] && g->line[i] != ' ')
		i++;
	if (g->line[i] == '#')
		return (parse_error(1, "newline", g));
	if (g->line[i] == '|' && g->line[i + 1] == '|')
		return (parse_error(1, "||", g));
	if (g->line[i] == '&' || g->line[i] == '(' || g->line[i] == ')'
			|| g->line[i] == ';' || g->line[i] == '|')
		return (parse_error(1, &g->line[i], g));
	if (g->line[i] == '\0')
		return (parse_error(1, "newline", g));
	if (g->line[i] == '<')
		return (norm_1(i, g, 1));
	if (g->line[i] == '>')
		return (norm_1(i, g, 0));
	return (0);
}
