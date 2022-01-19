/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseur_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:54:24 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 18:55:01 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	norm_parseur_2_3(t_ms *g, int i, int res, int in_out)
{
	if (in_out == 0)
	{
		res = parsing_redirection_out(i, res, g);
		if (res != 0)
			return (res);
	}
	else if (in_out == 1)
	{
		res = parsing_redirection_in(i, res, g);
		if (res != 0)
			return (res);
	}
	else if (in_out == 2)
	{
		res = parsing_pipe(i, res, g);
		if (res != 0)
			return (res);
	}
	return (0);
}

static int	norm_parsing_4(t_ms *g)
{
	record_list(&g->error, "bash: syntax error: unexpected end of file");
	g->ret_errno = 2;
	return (-1);
}

int	parseur(t_ms *g, int i, int res)
{
	(void)res;
	while (g->line[++i])
	{
		if (g->line[i] == '\'' || g->line[i] == '"')
		{
			i = parseur_quotes(g, i + 1, g->line[i]);
			if (i == -1)
				return (norm_parsing_4(g));
		}
		if (g->line[i] == '>')
			if (norm_parseur_2_3(g, i, 0, 0))
				return (1);
		if (g->line[i] == '<')
			if (norm_parseur_2_3(g, i, 0, 1))
				return (1);
		if (g->line[i] == '|')
			if (norm_parseur_2_3(g, i, 0, 2))
				return (1);
		if (g->line[i] == '\\')
			i++;
	}
	return (0);
}
