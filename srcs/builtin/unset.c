/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 22:30:20 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_unset_error(char *str)
{
	int	i;

	i = 1;
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
	{
		printf("minishell: uset: '%s': not a valid identifier\n", str);
		return (1);
	}
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
		{
			printf("minishell: unset: '%s': not a valid identifier\n", str);
			return (1);
		}
		i++;
	}
	return (0);
}

void	ft_unset(t_ms *g, int i)
{
	t_list	*tmp;
	char	*pos;
	t_list	*pre;

	tmp = g->env;
	g->ret_errno = 0;
	g->unset_path = 0;
	if (!g->cmd_tab[1])
		return ;
	if (check_unset_error(g->cmd_tab[1]))
	{
		g->ret_errno = 1;
		return ;
	}
	if (ft_strequ("PATH", g->cmd_tab[1]))
		g->unset_path = 1;
	while (g->cmd_tab[++i])
	{
		while (tmp)
		{
			pos = ft_strstr(tmp->content, g->cmd_tab[i]);
			if (pos && pos - (char *)tmp->content == 0)
			{
				if (((char *)tmp->content)[ft_strlen(g->cmd_tab[i])] == '=')
				{
					if (tmp->next)
						pre->next = tmp->next;
					else
						pre->next = NULL;
					free(tmp->content);
					free(tmp);
					break ;
				}
			}
			pre = tmp;
			tmp = tmp->next;
		}
		tmp = g->env;
	}
}
