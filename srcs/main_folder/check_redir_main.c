/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:59:00 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 19:01:01 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_spaceredir(char *str, char *tmp, int idx, int i)
{
	int	j;

	j = 0;
	while (str[i])
	{
		if (i == idx || i == idx + 1)
		{
			tmp[j] = ' ';
			j++;
		}
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	free(str);
	str = ft_strdup(tmp);
	free(tmp);
	tmp = NULL;
	return (str);
}

char	*ft_checkbackredir(t_ms *g, int i, int nb)
{
	char	*tmp;

	tmp = NULL;
	while (g->line[++i])
	{
		if (g->line[i] == '\'' || g->line[i] == '"')
			i = parseur_quotes(g, i + 1, g->line[i]);
		while (g->line[i] == '\\')
		{
			nb++;
			i++;
		}
		if ((g->line[i] == '>' || g->line[i] == '<') && nb != 0 && nb % 2 == 0)
		{
			tmp = (char *)malloc(sizeof(char) * (ft_strlen(g->line) + 3));
			if (!tmp)
				return (NULL);
			g->line = ft_spaceredir(g->line, tmp, i, 0);
			free(tmp);
			i += 1;
		}
		else
			nb = 0;
	}
	return (g->line);
}

char	*check_in_out(t_ms *g, char *str)
{
	if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		str = ft_checkredir(str);
	if (ft_strchr(str, '\\'))
		str = ft_checkbackredir(g, -1, 0);
	return (str);
}
