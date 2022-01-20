/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 01:03:49 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		i++;
	}
	return (i);
}

int	handle_cmd_noneed_fork(t_ms *g, char *cmd)
{
	if (!ft_strcmp(g->cmd_tab[0], "exit"))
	{
		ft_exit_plus(g->cmd_tab);
		free_split(g->cmd_tab);
		g->exit = 1;
		ft_exit(2, g, g->ret, cmd);
	}
	g->ret_errno = 0;
	if (ft_strcmp(g->cmd_tab[0], "export") == 0
		|| ft_strcmp(g->cmd_tab[0], "unset") == 0
		|| ft_strcmp(g->cmd_tab[0], "cd") == 0)
	{
		if (launch(cmd, g->cmd_tab[0], g, g->path[0]) == -1)
			perror("launch error");
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}

char	*find_cmd_in_path_tab(t_ms *g)
{
	int		i;
	char	*path_i;

	i = 0;
	while (g->path[i])
	{
		path_i = find_cmd_in_path_i(g->cmd_tab[0], g->path[i]);
		if (path_i)
			return (path_i);
		i++;
	}
	return (NULL);
}
