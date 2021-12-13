/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/13 12:55:18 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*check_option(char *line)
{
	int		i;
	char	*option;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '-')
			option = ft_strdup(line + i);
	}
	return (option);
}

int		parse_line(t_ms *g)
{
	int		i;
	int		x;
	char	*line;
	char	*option;
	t_list	*tmp;
	char	**line_split;
	char 	*tmp;
	
	line_split = NULL;
	g->nb_cmd = ft_lstsize(g->cmd_tmp);
	i = 0;
	x = -1;
	line = NULL;
	tmp = NULL;
	option = NULL;
	tmp = g->cmd_tmp;
	// g->parsing = malloc(sizeof(t_data) * nb_cmd);
	// if (!g->parsing)
		// ft_exit(0, g);
	while (tmp)
	{
		x = -1;
		i = -1;
		line = ft_strdup(tmp->content);
		line_split = ft_split_charset(line, " \t");
		// printf(_RED"%s\n"_NC, line);
		// printf(_GREEN"%s\n"_NC, g->line);
		while (line_split[++x])
		{
			option = check_option(line_split[x]);
			if (option)
			{
				tmp = ft_strjoin(line_split[x], " ");
				record_list(&g->cmd, ft_strjoin(tmp, option));
				ft_del_line(tmp);
			}
			ft_del_line(line_split[x]);
			ft_del_line(option);
		}
		free(line_split);
		ft_del_line(line);
		line_split = NULL;
		tmp = tmp->next;
	}
	ft_lstclear(&g->cmd_tmp, &ft_del_list);
	return(0);
}

int 	first_command(t_ms *g)
{
	int	res;
	// res = parse_line(line);
	res = parse_line(g);
	// if (check_bultin_in(g))

	// else if (check_biltin_out(g))

	// else
		// record_list(g->error, "Erreur\nCommande introuvable\n");
	return (res);
}

char	*get_cmd_in_line_th(char *line, t_ms *g)
{
	char	**line_split;
	int		i;
	(void)g;
	line_split = ft_split_charset(line, "|");
	i = -1;
	while (line_split[++i])
	{
		record_list(&g->cmd_tmp, line_split[i]);
		ft_del_line(line_split[i]);
	}
	free(line_split);
	return (line);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd;
	t_ms	g;

	(void)argv;
	cmd = NULL;
	if (argc != 1)
		return(printf(_RED"Error number arguments\n"_NC));
	signal(SIGINT, signal_in);
	begin(env, &g);
	while (1)
	{
		ft_putstr(_GREEN"thhusser> "_NC);
		if (!get_next_line(0, &g.line) || !ft_strcmp(g.line, "exit"))
			ft_exit(2, &g);
		cmd = get_cmd_in_line_th(g.line, &g);
		if (first_command(&g))
			ft_exit(0, &g);
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
	return (0);
}

