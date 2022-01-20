/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 01:05:32 by thhusser         ###   ########.fr       */
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

static int	fix_bug_quotes(char *cmd, t_ms *g)
{
	if (reset_cmd_variable(&cmd, g) == 1)
		return (1);
	g->cmd_tab = creat_list_arg(cmd);
	g->cmd_ac = count_tab(g->cmd_tab);
	test_redir_flag(cmd, g);
	init_redir(g, cmd);
	return (0);
}

static int	norm_fix_bug(char *cmd, t_ms *g)
{
	if (handle_cmd_noneed_fork(g, cmd) == 1)
		return (1);
	if (exec_cmd_has_dir(cmd, g->cmd_tab[0], g) == 1)
	{
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}

int	find_cmd_path(char *cmd, t_ms *g)
{
	char	*path_i;
	char	*str;
	char	*tmp;
	int		count;

	tmp = NULL;
	str = NULL;
	if (fix_bug_quotes(cmd, g))
		return (1);
	if (norm_fix_bug(cmd, g))
		return (1);
	path_i = find_cmd_in_path_tab(g);
	count = count_split(g->cmd_tab);
	str = norm_clean_line_cmd(g->cmd_tab, count, str, tmp);
	if (path_i)
	{
		launcher(str, g->cmd_tab[0], g, path_i);
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}
