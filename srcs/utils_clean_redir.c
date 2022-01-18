/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_clean_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:05:55 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 17:07:54 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tmp_che(char *tmp, char *str, int *i, int *j)
{
	int		c;

	c = str[*i - 1];
	while (str[*i] != c)
	{
		if ((str[*i] == '>' || str[*i] == '<'))
		{
			if (c == '"')
			{
				tmp[*j] = '\\';
				*j += 1;
			}
			tmp[*j] = '\\';
			*j += 1;
		}
		tmp[*j] = str[*i];
		*j += 1;
		*i += 1;
	}
	return (tmp);
}
