/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_double_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:34:25 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 17:36:00 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**cmd_creatnull(char **cmd, int j, int k)
{
	if (cmd[j] != NULL)
	{
		cmd[j][k] = '\0';
		cmd[j + 1] = NULL;
	}
	return (cmd);
}

int	ft_passpace(char *line, int idx)
{
	while (line[idx] == ' ')
		idx++;
	if (line[idx] == '\0')
		return (0);
	return (1);
}

void	print_split(char **cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
		printf("->%s-<\n", cmd[i]);
}

int	count_word(char *line)
{
	int		i;
	int		nb;

	i = 0;
	nb = 1;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
			i = parseur_quotes_str(line, i, line[i]);
		if (line[i] == ' ')
		{
			while (line[i + 1] == ' ')
				i++;
			nb++;
		}
		i++;
	}
	return (nb);
}
