/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:00:33 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 19:24:40 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	norm_1(int res, t_ms *g, int comp)
{
	int	tmp;

	tmp = 0;
	if (comp == 1)
	{
		while (g->line[tmp] && g->line[tmp] == ' ')
		{
			if (g->line[tmp] == '|' && g->line[tmp + 1] == '|')
				return (parse_error(1, "||", g));
			tmp++;
		}
		return (parse_error(1, "|", g));
	}
	if (g->line[res + 1] == '\0')
		return (parse_error(2, "multi", g));
	if (g->line[res + 1] == '|' && g->line[res + 2] == '|'
		&& g->line[res + 3] == '\0')
		return (parse_error(1, "||", g));
	if (g->line[res + 1] == '|')
		return (parse_error(1, "||", g));
	return (parse_error(2, "multi", g));
}

static int	norm_2(int res, t_ms *g)
{
	if (g->line[res] == '\0')
		return (parse_error(2, "multi", g));
	if (g->line[res] == '|') // pas tres juste tout ca ! A verifier en temps voulus
		return (norm_1(res, g, 0));
	if (g->line[res] == '!')
		return (parse_error(1, "!", g));
	if (g->line[res] == ';')
		return (parse_error(1, ";", g));
	if (g->line[res] == ')')
		return (parse_error(1, ")", g));
	if (g->line[res] == '}')
		return (parse_error(1, "}", g));
	if (g->line[res] == '&' || g->line[res] == '#' || g->line[res] == '('
		|| g->line[res] == '{')
		return (parse_error(2, "multi", g));
	return (0);
}

int		parsing_pipe(int i, int res, t_ms *g)
{
	if (g->line[i + 1] == '\0')
		return (parse_error(2, "multi", g));
	if (g->line[0] == '|' && g->line[1] == '|')
		return (parse_error(1, "||", g));
	if (g->line[i + 1] == '|' && g->line[i + 2] == '|' && g->line[i + 3] == '|')
		return (parse_error(1, "||", g));
	if (g->line[i + 1] == '|' && g->line[i + 2] == '|')
		return (parse_error(1, "|", g));
	if (g->line[i + 1] == '|' && g->line[i + 2] == '&')
		return (parse_error(1, "&", g));
	if (g->line[i + 1] == '&')
		return (parse_error(2, "multi", g));
	if (i == 0 && g->line[i + 1] == '|')
		return (parse_error(1, "||", g));
	if (i == 0)     //check si rencontree pre miere position alors erreur 
		return (parse_error(1, "|", g));
	res = i - 1;
	while (g->line[res] == ' ' && res > 0)
		res--;
	if (res == 0 && g->line[0] == ' ')
		return (norm_1(res, g, 1));
	res = i + 1;
	while (g->line[res] && g->line[res] == ' ')
		res++;
	return (norm_2(res, g));
}