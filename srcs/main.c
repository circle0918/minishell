/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/01/17 03:22:11 by thhusser         ###   ########.fr       */
=======
/*   Updated: 2022/01/17 01:58:54 by thhusser         ###   ########.fr       */
>>>>>>> fix_parsing_redirection
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
				record_list(&g->error, "bash: syntax error: unexpected end of file"); //mieux gerer les erreurs avec une fonction qui record l'erreur le char en question et le numero errno !
				g->ret_errno = 2;
				return (1);																//generer une erreur correspondante a bash
			}
		}
		// check chevron in et out // check si besoin de rechercher les >> et << ou parser les > + 1 et < + 1 (>+1 <+1 retenue !)
		if (g->line[i] == '>')
		{
			res = parsing_redirection_out(i, 0, g);
			if (g->line[i] == '>')
				i++;
			if (res != 0)
				return (res);
		}
		if (g->line[i] == '<')
		{
			res = parsing_redirection_in(i, 0, g);
			if (g->line[i + 1] == '<')
				i++;
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
	char *tmp;
	int count;

	str = NULL;
	tmp = NULL;
	i = -1;
	dest = ft_split(g->line, ' ');
	count = count_split(dest);
	while (dest[++i])
	{
		if (i == 0)
			str = ft_strjoin(dest[i], "");
		else if (count - 1 == i)
		{
			tmp = ft_strjoin(str, " ");
			ft_del_line(str);
			str = ft_strjoin(tmp, dest[i]);
			ft_del_line(tmp);
			break;
		}
		else if (i != 0)
		{
			tmp = ft_strjoin(str, " ");
			ft_del_line(str);
			str = ft_strjoin(tmp, dest[i]);
			ft_del_line(tmp);
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
			ft_lstclear(&g->env, &ft_del_list);
			ft_putstr(command);
			ft_putstr(": command not found\n");
			if (g->ret_errno == 0)
				g->ret_errno = 127;
		}
		else
			g->ret_errno = 0;
	}
	// free(command);
	return (0);
}

void	end(int sig)
{
	(void)sig;
	if (g_ms->pid[0] != 0 && g_ms->pid[1] == 0)
	{
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
		g_ms->ret_errno = 131;
	}
	else
	{
		ft_putstr("\b \b\b \b");
		g_ms->ret_errno = 1;
	}
}

void	recovery(int sig)
{
	(void)sig;
	if (g_ms->pid[0] == 0 && g_ms->pid[1] == 0)
	{
		ft_putstr("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_ms->ret_errno = 1;
	}
	else
	{
		if (g_ms->pid[1] != 0)
			kill(g_ms->pid[1], SIGINT);
		else
			kill(g_ms->pid[0], SIGINT);
		write(1, "\n", 1);
		g_ms->ret_errno = 130;
	}
}

int	contains_only_whitespaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' '
			&& line[i] != '\t'
			&& line[i] != '\r'
			&& line[i] != '\v')
			return (1);
		i++;
	}
	return (0);
}

char	*extract_string(char *str, int len)
{
	char	*res;
	int		i;

	i = 0;
	if (len == -1)
		len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 1));
	if (!(res))
		return (NULL);
	while (str[i] && i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int main(int argc, char **argv, char **env)
{
	char *cmd;
	t_ms g;

	(void)argv;
	cmd = NULL;
	g_ms = &g;
	if (argc != 1)
		return (printf(_RED "Error number arguments\n"_NC));
	// signal(SIGINT, signal_in);
	// signal(SIGQUIT, end);
	signal(SIGINT, recovery);
	signal(SIGQUIT, end);
	begin(env, &g);
	g.ret = 1;
	while (1)
	{
		init_pipe(&g);
		g.line = readline(_GREEN "thhusser> "_NC);
		if (!g.line)
			g.line = extract_string("exit", -1);
		if (*(g.line) != '\0'  && contains_only_whitespaces(g.line))
		{
			add_history(g.line);
			clean_command(&g);
		}
		if (g.error)
		{
			print_list(g.error);
			ft_lstclear(&g.error, &ft_del_list);
		}
		// ft_del_line(g.line);
	}
	return (0);
}
