/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 22:49:56 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_cmd_in_path_i(char *cmd, char *path_i)
{
	DIR				*dir;
	struct dirent	*dirp;
	char			*ret;

	dir = opendir(path_i);
	if (dir)
	{
		dirp = readdir(dir);
		while (dirp != NULL)
		{
			if (ft_strequ(dirp->d_name, cmd))
			{
				ret = path_i;
				closedir(dir);
				return (ret);
			}
			dirp = readdir(dir);
		}
	}
	closedir(dir);
	return (NULL);
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

int	reset_cmd_variable(char **cmd, t_ms *g)
{
	if (ft_strchr(*cmd, '$'))
	{
		*cmd = check_var_cmd(g, *cmd);
		if (!cmd || ft_strequ(*cmd, "\0"))
			return (1);
	}
	return (0);
}

int	find_cmd_path(char *cmd, t_ms *g)
{
	char	*path_i;

	if (reset_cmd_variable(&cmd, g) == 1)
		return (1);
	g->cmd_tab = creat_list_arg(cmd);
	g->cmd_ac = count_tab(g->cmd_tab);
	test_redir_flag(cmd, g);
	init_redir(g, cmd);
	if (handle_cmd_noneed_fork(g, cmd) == 1)
		return (1);
	if (exec_cmd_has_dir(cmd, g->cmd_tab[0], g) == 1)
	{
		free_split(g->cmd_tab);
		return (1);
	}
	path_i = find_cmd_in_path_tab(g);
	if (path_i)
	{
		launcher(cmd, g->cmd_tab[0], g, path_i);
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}
