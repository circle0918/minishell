/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 14:35:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/12 14:37:56 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		move_space_after(char *str, int i)
{
	while (str[i + 1] == ' ')
		i++;
	return (i);
}

int		move_space_before(char *str, int c)
{
	while (str[c - 1] == ' ')
		c--;
	return (c);
}
