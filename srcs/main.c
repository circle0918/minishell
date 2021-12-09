/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/09 12:32:37 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(int nb, char *line)
{
	ft_del_line(line);
	exit(nb);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	
	line = NULL;
	(void)argc;
	(void)argv;
	(void)env;
	while (1)
	{
		ft_putstr(_GREEN"thhusser> "_NC);
		if (!get_next_line(0, &line))
			ft_exit(1, line);
		ft_del_line(line);
	}
	return (0);
}
