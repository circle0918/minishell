/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/10 15:20:36 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	free_split(char **split)
{
	int i;

	i = -1;
	if (split)
	{
		while (split[++i])
		{
			if (split[i])
				free(split[i]);
		}
		free(split);
	}
}

void	ft_exit(int nb, char *line, t_ms *g)
{
	if (nb == 2)
		ft_putstr("exit\n");	
	ft_del_line(line);
	ft_lstclear(&g->env, &ft_del_list);
	ft_lstclear(&g->cmd, &ft_del_list);
	ft_lstclear(&g->cmd_tmp, &ft_del_list);
	ft_lstclear(&g->error, &ft_del_list);
	free_split(g->path);
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

void	begin(char **env, t_ms *g)
{
	int i;

	init_global_struct(g);
	i = -1;
	while (env[++i])
		record_list(&g->env, env[i]);
	get_path(g);
}

void	signal_in(int signal)
{
	(void)signal;
	ft_putstr("\n");
	ft_putstr(_GREEN"thhusser> "_NC);
}

void	test(int signal)
{
	printf("%d\n",signal);	
}

char	*get_cmd_in_line_th(char *line, t_ms *g)
{
	char	**line_split;
	int		i;
	(void)g;
	line_split = ft_split_charset(line, " \t");
	i = -1;
	while (line_split[++i])
	{
		record_list(&g->cmd_tmp, line_split[i]);
		ft_del_line(line_split[i]);
	}
	free(line_split);
	return (line);
}

#define TEST 0
int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_ms	g;

	(void)argv;
	cmd = NULL;
#if TEST == 0
	if (argc != 1)
		return(printf(_RED"Error number arguments\n"_NC));
#else
	(void)argc;
#endif
	signal(SIGINT, signal_in);
	signal(SIGQUIT, test);
	begin(env, &g);
#if TEST == 0
	while (1)
	{
		ft_putstr(_GREEN"thhusser> "_NC);
		if (!get_next_line(0, &g.line) || !ft_strcmp(g.line, "exit"))
			ft_exit(2, g.line, &g);
		cmd = get_cmd_in_line_th(g.line, &g);
		// print_list(g.cmd_tmp);
		if (!find_cmd_path(cmd, &g))
		{
			ft_putstr("minishell: ");
			ft_putstr(cmd);
			ft_putstr(": command not found\n");
		}
		if (!ft_strcmp(g.line, "env"))
			print_list(g.env);
		ft_del_line(g.line);
	}
#else
		g.line = ft_strdup(argv[2]);
		cmd = get_cmd_in_line_th(g.line, &g);
		// print_list(g.cmd_tmp);
		if (!find_cmd_path(cmd, &g))
		{
			ft_putstr("minishell: ");
			ft_putstr(cmd);
			ft_putstr(": command not found\n");
		}
		if (!ft_strcmp(g.line, "env"))
			print_list(g.env);
		ft_del_line(g.line);
#endif	
	return (0);
}

