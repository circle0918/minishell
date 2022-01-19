/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:32:51 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/17 01:58:14 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_argc_redir(char **tab)
{
	int	i;
	int	argc;

	i = 0;
	argc = 1;
	while (tab[i] && tab[i + 1])
	{
		if (ft_strequ(tab[i + 1], ">") || ft_strequ(tab[i + 1], ">>"))
		{
			if (ft_strequ(tab[i], "1")
				|| ft_strequ(tab[i], "2") || ft_strequ(tab[i], "&"))
				argc = i;
			else
				argc = i + 1;
			break ;
		}
		else if (ft_strequ(tab[i + 1], "<") || ft_strequ(tab[i + 1], "<<"))
		{
			argc = i + 1;
			break ;
		}
		i++;
		argc++;
	}
	return (argc);
}

char	**get_argv_redir(char *cmd)
{
	char	**tab;
	char	**argv;
	int		argc;
	int		i;

	tab = creat_list_arg(cmd);
	argc = get_argc_redir(tab);
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	argv[argc] = NULL;
	i = 0;
	while (i < argc)
	{
		argv[i] = ft_strdup(tab[i]);
		i++;
	}
	free_split(tab);
	return (argv);
}

char	**get_env_tab(t_list *env)
{
	t_list	*l;
	char	**ret;
	int		i;

	ret = (char **)malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	i = 0;
	l = env;
	while (l)
	{
		ret[i++] = ft_strdup((char *)(l->content));
		l = l->next;
	}
	ret[i] = NULL;
	return (ret);
}

int	handle_redir_in(int *fd, char **tab, int i)
{
	if (*fd > 0)
		close(*fd);
	*fd = open(tab[i + 1], O_RDONLY);
	if (*fd < 0)
	{
		error_out2(NULL, tab[i + 1], "No such file or directory");
		free_split(tab);
		g_ms->ret_errno = 1;
		return (-1);
	}
	return (0);
}
