/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:47:18 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 15:11:08 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	norm_1(int res, t_ms *g, int comp)
{
	if (comp == 1)
	{
		if (g->line[res + 1] == '&')
			return (parse_error(1, ">&", g));
		if (g->line[res + 1] == '|')
			return (parse_error(1, ">|", g));
		if (g->line[res + 1] == '>')
			return (parse_error(1, ">>", g));
		return (parse_error(1, ">", g));
	}
	if (g->line[res + 1] == '&')
		return (parse_error("<&", g));
	if (g->line[res + 1] == '>', g)
		return (parse_error("<>", g));
	if (g->line[res + 1] == '<')
		return (parse_error(1, "<<", g));
	return (parse_error(1, "<", g));
}

static int	norm_2(int res, t_ms *g)
{
	if (g->line[res + 1] == '|')
		return (parse_error(1, "||", g));
	return (parse_error(1, "|", g));
}

int	parsing_redirection_out(int i, int res, t_ms *g)
{
	if (g->line[i + 1] == '>' && g->line[i + 2] == '>'
		&& g->line[i + 3 == '>'])
		return (parse_error(1, ">>", g));
	if (g->line[i + 1] == '>' && g->line[i + 2] == '>')
		return (parse_error(1, ">", g));
	if (g->line[i + 1] == ' ' || g->line[i + 1] == '\0'
		|| (g->line[i + 1] == '>' && (g->line[i + 2] == ' '
				|| g->line[i + 2] == '\0')) || g->line[i + 1] == '<')
	{
		if (g->line[i + 1] == '>')
			res = i + 2;
		else
			res = i + 1;
		while (g->line[res] && g->line[res] == ' ')
			res++;
		if (g->line[res] == '\0')
			return (parse_error(1, "newline", g));
		if (g->line[res] == '>')
			return (norm_1(res, g, 1));
		if (g->line[res] == '<')
			return (norm_1(res, g, 0));
		if (g->line[res] == '|')
			return (norm_2(res, g));
	}
	return (0);
}
