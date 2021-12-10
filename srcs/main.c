/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:25:01 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/10 12:10:27 by thhusser         ###   ########.fr       */
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

void	get_path(t_ms *g)
{
	char *path;
	t_list *tmp;
	char *pos;

	tmp = g->env;
	path = NULL;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, "PATH=");
		if (pos && pos - (char *)tmp->content == 0)
		{
			path = tmp->content;
			break;
		}
		tmp = tmp->next;
	}
	// maybe path= null or PATH= nothing
	path = ft_substr(path, 5, (ft_strlen(path) - 5));
	g->path = ft_split(path, ':');
}

void	begin(int argc, char **argv, char **env, t_ms *g)
{
	(void)argc;
	(void)argv;
	int i;

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
char	*get_cmd_in_line(char *line)
{
	//char *cmd;
	
	//TODO PARSER ls -a but take only ls
	return (line);
}
int		find_cmd_path(char *cmd, t_ms *g)
{
	DIR				*dir;
	struct dirent	*dirp;
	int				i;

	i = 0;

	while (g->path[i])
	{
		dir = opendir(g->path[i]);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, cmd))
				{
					if (closedir(dir) == -1)
						perror("error : closedir");	
					return (1);
				}
			}
		}
		if (closedir(dir) == -1)
			perror("error : closedir");	
		i++;
	}
	return (0);
}
int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	*cmd;
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
		cmd = get_cmd_in_line(line);
		if (!find_cmd_path(cmd, &g))
		//	printf("minishell: %s: command not found", cmd);
		{
			ft_putstr("minishell: ");
			ft_putstr(cmd);
			ft_putstr(": command not found\n");
		}
		if (!ft_strcmp(line, "env"))
			print_list(g.env);
		ft_del_line(line);
	}
	return (0);
}

