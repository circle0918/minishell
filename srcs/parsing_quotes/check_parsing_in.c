/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:47:52 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/17 00:49:21 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	norm_1(int res, t_ms *g, int comp)
{
	if (comp)
	{
		if (g->line[res + 1] == '<' && g->line[res + 2] == '<')
			return (parse_error(1, "<<<", g));
		if (g->line[res + 1] == '<')
			return (parse_error(1, "<<", g));
		if (g->line[res + 1] == '&')
			return (parse_error(1, "<&", g));
		return (parse_error(1, "<", g));
	}
	if (g->line[res + 1] == '|')
		return (parse_error(1, ">|", g));
	if (g->line[res + 1] == '&')
		return (parse_error(1, ">&", g));
	if (g->line[res + 1] == '>')
		return (parse_error(1, ">>", g));
	return (parse_error(1, ">", g));
}

int		parsing_redirection_in(int i, int res, t_ms *g)
{
	(void)res;
	if (g->line[i + 1] == '<')
		i++;
	i++;
	if (g->line[i] == '<')
		return (parse_error(1, "<", g));
	while (g->line[i] && g->line[i] == ' ')
		i++;
	if (g->line[i] == '#')
		return (parse_error(1, "newline", g));
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
