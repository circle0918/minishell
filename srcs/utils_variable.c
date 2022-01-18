/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:58:59 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 17:59:25 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	checkvar(char *str, int i)
{
	if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '?')
		&& !ft_isdigit(str[i + 1]) && ft_strlen(str) != 1 && str[i + 1] != '\0'
		&& str[i + 1] != ' ')
		return (1);
	return (0);
}
