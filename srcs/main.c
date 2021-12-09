/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/09 17:25:09 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(int nb, char *line, t_ms *g)
{
	if (nb == 2)
		ft_putstr("exit\n");	
	ft_del_line(line);
	ft_lstclear(&g->env, &ft_del_list);
	exit(1);
}

void	print_list(t_list *error)
{
	char	*line;

	line = NULL;
	while (error)
	{
		line = ft_strdup(error->content);
		ft_putstr(line);
		ft_putstr("\n");
		free(line);
		error = error->next;
	}
}

void	record_list(t_list **list, char *str)
{
	t_list	*new_elem;

	new_elem = ft_lstnew(ft_strdup(str));
	ft_lstadd_back(list, new_elem);
}

void	begin(int argc, char **argv, char **env, t_ms *g)
{
	(void)argc;
	(void)argv;
	int i;

	i = -1;
	while (env[++i])
		record_list(&g->env, env[i]);
}

void	signal_in(int signal)
{
	(void)signal;
	ft_putstr("\n");
	ft_putstr(_GREEN"thhusser> "_NC);
}

void	test(int signal)
{
	
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	t_ms	g;

	line = NULL;
	g.env= NULL;
	signal(SIGINT, signal_in);
	signal(SIGQUIT, test);
	begin(argc, argv, env, &g);
	while (1)
	{
		ft_putstr(_GREEN"thhusser> "_NC);
		if (!get_next_line(0, &line) || !ft_strcmp(line, "exit"))
			ft_exit(2, line, &g);
		if (!ft_strcmp(line, "env"))
			print_list(g.env);
		ft_del_line(line);
	}
	return (0);
}
