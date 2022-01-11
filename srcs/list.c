/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:32:51 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/14 18:38:16 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	record_list(t_list **list, char *str)
{
	t_list	*new_elem;

	new_elem = ft_lstnew(ft_strdup(str));
	ft_lstadd_back(list, new_elem);
}

void	print_list(t_list *error)
{
	char	*line;

	line = NULL;
	while (error)
	{
		line = ft_strdup(error->content);
		ft_putstr(line);
		free(line);
		error = error->next;
	}
}
