/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 19:31:32 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*ft_lst_pop_last(t_list **lst)
{
	t_list	*pop;
	t_list	*top;

	if (lst == NULL || *lst == NULL)
		return (NULL);
	top = *lst;
	if (top->next == NULL)
	{
		pop = top;
		top = NULL;
		return (pop);
	}
	while (top->next->next)
		top = top->next;
	pop = top->next;
	top->next = NULL;
	return (pop);
}

int	recup_content(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '=')
		i++;
	return (i);
}

void	export_append(char **tab, int i, t_ms *g)
{
	t_list	*last;

	last = ft_lst_pop_last(&g->env);
	record_list(&g->env, tab[i]);
	ft_lstadd_back(&g->env, last);
}

int	ft_export_path_flag(t_ms *g)
{
	char	*strsub;

	g->ret_errno = 0;
	if (g->cmd_ac == 1)
	{
		export_no_arg(g);
		return (1);
	}
	strsub = ft_substr(g->cmd_tab[1], 0, 4);
	if (ft_strequ("PATH", strsub))
	{
		g->unset_path = 0;
	}
	free(strsub);
	return (0);
}

void	ft_export(t_ms *g)
{
	int		i;
	char	*ptr;

	if (ft_export_path_flag(g) == 1)
		return ;
	i = 0;
	while (g->cmd_tab[++i])
	{
		if (export_checker(g->cmd_tab, i, g))
			continue ;
		ptr = ft_strchr(g->cmd_tab[i], '=');
		if (ptr == NULL)
			continue ;
		else
		{
			if (export_replaced(ptr, g->cmd_tab, i, g))
				continue ;
			export_append(g->cmd_tab, i, g);
		}
	}
}
