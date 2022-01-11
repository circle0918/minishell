/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 10:07:49 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 15:33:00 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		parse_error(int res, char *erreur, t_ms *g)
{
	char	*line_1;
	char	*dest;

	line_1 = NULL;
	dest = NULL;
	if (res == 1)
	{
		line_1 = ft_strjoin("bash: syntax error near unexpected token `", erreur);
		dest = ft_strjoin(line_1, "'\n");
		ft_del_line(line_1);
		record_list(&g->error, dest);
		ft_del_line(dest);
		errno = 2;
	}
	else if (res == 2)
	{
		record_list(&g->error, "bash: missing quote\n");
		errno = 130;
		return (-1);
	}
	return (res);
}

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
