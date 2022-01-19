/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:22 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 18:53:51 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	begin(char **env, t_ms *g)
{
	int	i;

	init_global_struct(g);
	i = -1;
	while (env[++i])
		record_list(&g->env, env[i]);
	get_path(g);
}

int	count_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void	norm_clean_cmd(t_ms *g, char *command)
{
	ft_putstr(command);
	ft_putstr(": command not found\n");
	if (g->ret_errno == 0)
		g->ret_errno = 127;
}

int	contains_only_whitespaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' '
			&& line[i] != '\t'
			&& line[i] != '\r'
			&& line[i] != '\v')
			return (1);
		i++;
	}
	return (0);
}

char	*extract_string(char *str, int len)
{
	char	*res;
	int		i;

	i = 0;
	if (len == -1)
		len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 1));
	if (!(res))
		return (NULL);
	while (str[i] && i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
