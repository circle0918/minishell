/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 22:43:53 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_ms	g;

	(void)argv;
	cmd = NULL;
	g_ms = &g;
	if (argc != 1)
		return (printf(_RED "Error number arguments\n"_NC));
	signal(SIGINT, recovery);
	signal(SIGQUIT, end);
	begin(env, &g);
	while (1)
	{
		init_pipe(&g);
		g.line = readline(_GREEN "thhusser> "_NC);
		if (!g.line)
			g.line = extract_string("exit", -1);
		if (*(g.line) != '\0' && contains_only_whitespaces(g.line))
			lauch_all(&g);
		if (g.error)
			print_error_main(&g);
	}
	return (0);
}
