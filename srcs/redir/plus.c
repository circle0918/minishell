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

void	handle_redir_in_in(int *fd, char **tab, int i)
{
	char	*delimitor;
	char	*s;

	if (*fd > 0)
		close(*fd);
	delimitor = tab[i + 2];
	*fd = open("redir_lessless", O_CREAT | O_WRONLY | O_APPEND | O_TRUNC, 0644);
	while (1)
	{
		s = readline("> ");
		if (ft_strequ(s, delimitor))
		{
			free(s);
			break ;
		}
		ft_putstr_fd(s, *fd);
		ft_putstr_fd("\n",*fd);
		free(s);
	}
	close(*fd);
	*fd = open("redir_lessless", O_RDONLY);
}

void	handle_redir_out(int *fd, char **tab, int i)
{
	if (ft_strequ(tab[i], ">"))
	{
		if (*fd > 0)
			close(*fd);
		*fd = open(tab[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	}
	else if (ft_strequ(tab[i], ">>"))
	{
		if (*fd > 0)
			close(*fd);
		*fd = open(tab[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0664);
	}
}

int	get_redir_in_file(char *cmd)
{
	char	**tab;
	int		i;
	int		fd;

	fd = 0;
	tab = creat_list_arg(cmd);
	i = 0;
	while (tab[i] && tab[i + 1])
	{
		if (ft_strequ(tab[i], "<") && !ft_strequ(tab[i + 1], "<"))
		{
			if (handle_redir_in(&fd, tab, i) == -1)
				return (-1);
		}
		else if (ft_strequ(tab[i], "<")
			&& ft_strequ(tab[i + 1], "<") && tab[i + 2])
		{
			handle_redir_in_in(&fd, tab, i);
			i++;
		}
		i++;
	}
	free_split(tab);
	return (fd);
}

int	get_redir_out_file(char *cmd)
{
	char	**tab;
	int		i;
	int		fd;

	fd = 0;
	tab = creat_list_arg(cmd);
	i = 0;
	while (tab[i] && tab[i + 1])
	{
		handle_redir_out(&fd, tab, i);
		i++;
	}
	free_split(tab);
	if (fd <= 0)
		return (0);
	return (fd);
}
