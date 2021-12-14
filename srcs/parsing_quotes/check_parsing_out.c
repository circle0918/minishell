/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:47:18 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 12:58:13 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		parsing_redirection_out(int i, int res, t_ms *g)
{
	if (g->line[i + 1] == '>' && g->line[i + 2] == '>' && g->line[i + 3 == '>'])
		return (parse_error(1, ">>")); // fonction a gerer, supprimer la liste et le code 1 ou 2 selon l'erreur de shell
	if (g->line[i + 1] == '>' && g->line[i + 2] == '>')
		return (parse_error(1, ">"));
	if (g->line[i + 1] == ' ' || g->line[i + 1] == '\0' || (g->line[i + 1] == '>' &&
		(g->line[i + 2] == ' ' || g->line[i + 2] == '\0')) || g->line[i + 1] == '<')
	{
		if (g->line[i + 1] == '>')
			res = i + 2;
		else
			res = i + 1;
		while (g->line[res] && g->line[res] == ' ')
			res++;
		if (g->line[res] == '\0')
			return (parse_error(1, "newline"));
		if (g->line[res] == '>')
		{

			if (g->line[res + 1] == '&')
				return (parse_error(1, ">&"));
			if (g->line[res + 1] == '|')
				return (parse_error(1, ">|"));
			if (g->line[res + 1] == '>')
				return (parse_error(1, ">>"));
			return (parse_error(1, ">"));
		}
		if (g->line[res] == '<')
		{
			if (g->line[res + 1] == '&')
				return (parse_error("<&"));
			if (g->line[res + 1] == '>')
				return (parse_error("<>"));
			if (g->line[res + 1] == '<')
				return (parse_error(1, "<<"));
			return (parse_error(1, "<"));
		}
		if (g->line[res] == '|')
		{
			if (g->line[res + 1] == '|')
				return (parse_error(1, "|"));
			return (parse_error(1, "|"));
		}
	}
	return (0);
}