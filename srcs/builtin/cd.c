/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 19:04:36 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd_0(t_ms *g, char **path)
{
	if (g->cmd_ac == 2)
		*path = ft_strdup(g->cmd_tab[1]);
	g->ret_errno = 0;
	if (g->cmd_ac > 2)
	{
		g->ret_errno = 1;
		error_out2("cd", *path, "Too many arguements");
		return (1);
	}
	if (g->cmd_ac == 1 || ft_strequ(*path, "~") || ft_strequ(*path, "--"))
	{
		if (g->cmd_ac == 2)
			free(*path);
		*path = ft_strdup(get_env("HOME", g->env));
	}
	return (0);
}

int	ft_cd_1(t_ms *g, char **path)
{
	char	*tmp_path;

	if (ft_strequ(*path, "-"))
	{
		free(*path);
		*path = get_env("OLDPWD", g->env);
		if (chdir(*path) == 0)
			change_path(g, 1);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", *path, "No such file or directory");
		}
		return (1);
	}
	if ((*path)[0] == '~')
	{
		tmp_path = *path;
		*path = ft_strjoin(get_env("HOME", g->env), (tmp_path + 1));
		free(tmp_path);
	}
	return (0);
}

void	ft_cd_2(t_ms *g, char **path)
{
	char	cwd[1024];

	ft_memset(cwd, 0, 1024);
	getcwd(cwd, sizeof (cwd));
	ft_strcat(cwd, "/");
	ft_strcat(cwd, *path);
	if (chdir(cwd) == 0)
		change_path(g, 0);
	else
	{
		g->ret_errno = 1;
		error_out2("cd", *path, "No such file or directory");
	}
}

void	ft_cd(t_ms *g)
{
	char	*path;

	path = NULL;
	if (ft_cd_0(g, &path) == 1)
		return ;
	if (ft_cd_1(g, &path) == 1)
		return ;
	if (path[0] != '/')
		ft_cd_2(g, &path);
	else
	{
		if (chdir(path) == 0)
			change_path(g, 0);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", path, "No such file or directory");
		}
	}
	if (path)
		free(path);
}
