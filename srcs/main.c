/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/15 16:27:39 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//enrehistrement des variables envirenementales dans une liste !
void begin(char **env, t_ms *g)
{
	int i;

	init_global_struct(g);
	i = -1;
	while (env[++i])
		record_list(&g->env, env[i]);
	get_path(g);
}

//Signal pour le 'ctrl + C'
void signal_in(int signal)
{
	(void)signal;
	errno = 130;
	ft_putstr("\n");
	ft_putstr(_GREEN "thhusser> "_NC);
}

int parseur(t_ms *g, int i, int res)
{
	while (g->line[++i])
	{
		//check double cote ou simple cote
		if (g->line[i] == '\'' || g->line[i] == '"') // les passer avec les fonctions et retourner i pour decaller l'index
		{
			i = parseur_quotes(g, i + 1, g->line[i]);
			if (i == -1)
			{
				record_list(&g->error, "bash: syntax error: unexpected end of file\n"); //mieux gerer les erreurs avec une fonction qui record l'erreur le char en question et le numero errno !
				errno = 2;
				return (1);																//generer une erreur correspondante a bash
			}
		}
		// check chevron in et out // check si besoin de rechercher les >> et << ou parser les > + 1 et < + 1 (>+1 <+1 retenue !)
		if (g->line[i] == '>')
		{
			res = parsing_redirection_out(i, 0, g);
			if (res != 0)
				return (res);
		}
		if (g->line[i] == '<')
		{
			res = parsing_redirection_in(i, 0, g);
			if (res != 0)
				return (res);
		}
		// check pipe (compter nombre de pipe ? compter nombre de sous commande ? utiliser les global pour le multi pipe ?)
		if (g->line[i] == '|')
		{
			res = parsing_pipe(i, 0, g);
			if (res != 0)
				return (res);
		}
		//si back slash i++
		if (g->line[i] == '\\')
			i++;
		//efectuer les fonction de test et return res pour les erreurs
		//chercher ou mettre les commande $ ... pour echo ! --> une fois parser ! apres test si pipe
	}
	return (0);
}

static void clean_line(t_ms *g)
{
	int i;
	char *dest;

	i = 0;
	while (g->line[i] && g->line[i] == ' ')
		i++;
	dest = ft_strdup(g->line + i);
	ft_del_line(g->line);
	g->line = dest;
}

char *ft_spaceredir(char *str, char *tmp, int idx, int i)
{
	int j;

	j = 0;
	while (str[i])
	{
		if (i == idx || i == idx + 1)
		{
			tmp[j] = ' ';
			j++;
		}
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	free(str);
	str = ft_strdup(tmp);
	free(tmp);
	tmp = NULL;
	return (str);
}

char *ft_checkbackredir(t_ms *g, int i, int nb)
{
	char *tmp;

	tmp = NULL;
	while (g->line[i])
	{
		if (g->line[i] == '\'' || g->line[i] == '"')
			i = parseur_quotes(g, i + 1, g->line[i]);
		while (g->line[i] == '\\')
		{
			nb++;
			i++;
		}
		if ((g->line[i] == '>' || g->line[i] == '<') && nb != 0 && nb % 2 == 0)
		{
			if (!(tmp = (char *)malloc(sizeof(char) * (ft_strlen(g->line) + 3))))
				return (NULL);
			g->line = ft_spaceredir(g->line, tmp, i, 0);
			i += 1;
		}
		else
			nb = 0;
		i++;
	}
	return (g->line);
}

char *check_in_out(t_ms *g, char *str)
{
	if (ft_strchr(str, '>') || ft_strchr(str, '<'))
		str = ft_checkredir(str);
	if (ft_strchr(str, '\\'))
		str = ft_checkbackredir(g, 0, 0);
	return (str);
}

//check_in_out  --> my_redirection
//check_nb_pipe --> ft_nbpipe2
//pipe_command  --> ft_pipe
//command_exec  --> ft_command

int check_nb_pipe(const char *str, t_ms *g)
{
	int i;
	int nb;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if ((i = parseur_quotes(g, i + 1, str[i])) == -1)
				break;
		}
		if (str[i] == '|')
			nb++;
		if (str[i] == '\\')
			i++;
		i++;
	}
	return (nb);
}

int count_split(char **split)
{
	int i;

	i = 0;
	while (split[i])
		i++;
	return (i);
}

void clean_line_cmd(t_ms *g)
{
	char **dest;
	int i;
	char *str;
	int count;

	str = NULL;
	i = -1;
	dest = ft_split(g->line, ' ');
	count = count_split(dest);
	while (dest[++i])
	{
		if (i == 0)
		{
			str = ft_strjoin(dest[i], "");
		}
		else if (count - 1 == i)
		{
			str = ft_strjoin(str, " ");
			str = ft_strjoin(str, dest[i]);
			break;
		}
		else if (i != 0)
		{
			str = ft_strjoin(str, " ");
			str = ft_strjoin(str, dest[i]);
		}
	}
	free_split(dest);
	ft_del_line(g->line);
	g->line = str;
}

int clean_command(t_ms *g)
{
	int i;
	int pipe;
	char *command;

	command = NULL;
	i = -1;
	pipe = 0;
	if (parseur(g, -1, 0)) // envoie i a -1 et le comteur d'erreur a 0
		return (1);
	clean_line(g);
	clean_line_cmd(g);
	if (g->line)
	{
		command = check_in_out(g, g->line);
		pipe = check_nb_pipe(command, g);
		if (pipe)
			pipe_command(g, pipe);
		else if (!find_cmd_path(command, g)) // --> lancement partie yyuan
		{
			ft_putstr(command);
			ft_putstr(": command not found\n");
			errno = 127;
		}
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	char *cmd;
	t_ms g;

	(void)argv;
	cmd = NULL;
	if (argc != 1)
		return (printf(_RED "Error number arguments\n"_NC));
	signal(SIGINT, signal_in);
	begin(env, &g);
	while (1)
	{
		init_pipe(&g);
		g.line = readline(_GREEN "thhusser> "_NC);
		if (!ft_strequ(g.line, ""))
			add_history(g.line);
		if (!g.line || !ft_strcmp(g.line, "exit"))
			ft_exit(2, &g);
		clean_command(&g);
		if (g.error)
		{
			print_list(g.error);
			ft_lstclear(&g.error, &ft_del_list);
		}
	}
	return (0);
}
