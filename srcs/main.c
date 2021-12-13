/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/13 17:35:50 by thhusser         ###   ########.fr       */
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
	char 	*tmp_line;
	
	line_split = NULL;
	g->nb_cmd = ft_lstsize(g->cmd_tmp);
	i = 0;
	x = -1;
	line = NULL;
	tmp_line = NULL;
	option = NULL;
	tmp = g->cmd_tmp;
	// g->parsing = malloc(sizeof(t_data) * nb_cmd);
	// if (!g->parsing)
		// ft_exit(0, g);
	printf(_GREEN"%s\n"_NC, g->line);
	while (tmp)
	{
		x = -1;
		i = -1;
		line = ft_strdup(tmp->content);
		// line_split = ft_split_charset(line, " \t");
		printf(_RED"%s\n"_NC, line);
		// while (line_split[++x])
		{
			// option = check_option(line_split[x]);
			// if (option)
			// {
			// 	tmp_line = ft_strjoin(line_split[x], " ");
			// 	record_list(&g->cmd, ft_strjoin(tmp_line, option));
			// 	ft_del_line(tmp_line);
	 		// 	ft_del_line(option);
			// }
			// ft_del_line(line_split[x]);
		}
		// free(line_split);
		// line_split = NULL;
		ft_del_line(line);
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

int		parseur(t_ms *g, int i, int res)
{
	while (g->line[++i])
	{
		//check double cote ou simple cote
		if (g->line[i] == '\'' || g->line[i] == '"') // les passer avec les fonctions et retourner i pour decaller l'index
		{
			i = parseur_quotes(i + 1, g->line[i]);
			if (i == -1)
				return ();//generer une erreur correspondate a bash
		}
		// checek chevron in et out
		if (g->line[i] == '>')
		{
			res = parsing_redirection_out(i);
			if (res != 0)
				return (res);
		}
		if (g->line[i] == '<')
		{
			res = parsing_redirection_in(i);
			if (res != 0)
				return (res);
		}
		// check pipe
		if (g->line[i] == '|')
		{
			res = parsing_pipe(i);
			if (res != 0)
				return (res);
		}
		//si back slash i++
		if (g->line[i] == '\\')
			i++;
		//efectuer les fonction de test et return res pour les erreurs
		//chercher ou mettre les commande $ ... pour echo !
	}
	return(0);
}

void	clean_command(t_ms *g)
{
	int	i;
	int	pipe;

	i = -1;
	pipe = 0;
	if (parseur(g, -1, 0)) // envoie i a -1 et le comteur d'erreur a 0
		return (1);
	// parseur va check tous les padding probleme de cote ...
	// ensuite enlever tous les espace en debut de ligne
	// ensuite test sur la commande si pipe sinon commande a executer
	// --> ou alors boucle pour le nombre de pipe present dans la commande


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
		// cmd = get_cmd_in_line_th(g.line, &g);
		// if (first_command(&g))
		// 	ft_exit(0, &g);
		// // print_list(g.cmd_tmp);
		// if (!find_cmd_path(cmd, &g))
		// {
		// 	ft_putstr("minishell: ");
		// 	ft_putstr(cmd);
		// 	ft_putstr(": command not found\n");
		// }
		// if (!ft_strcmp(g.line, "env"))
		// 	print_list(g.env);
		clean_command(&g);
		ft_del_line(g.line);
	}
	return (0);
}

