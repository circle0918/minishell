/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 22:32:34 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_buildin_2(char *comd, t_ms *g)
{
	if (ft_strcmp(comd, "export") == 0)
	{
		ft_export(g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "unset") == 0)
	{
		ft_unset(g);
		return (1);
	}
	if (g->unset_path == 1)
		return (-1);
	if (ft_strcmp(comd, "env") == 0)
	{
		print_list(g->env);
		return (1);
	}
	else
		return (0);
}

int	is_buildin(char *comd, t_ms *g)
{
	if (ft_strcmp(comd, "pwd") == 0)
	{
		ft_pwd(g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "echo") == 0)
	{
		ft_echo(g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "cd") == 0)
	{
		ft_cd(g);
		return (1);
	}
	else
		return (is_buildin_2(comd, g));
}

char	**init_argv(char *cmd)
{
	int		j;
	char	**split_cmd;
	char	**argv;

	argv = (char **)malloc(sizeof(char *) * (get_cmd_size(cmd) + 1));
	split_cmd = ft_split(cmd, ' ');
	j = 0;
	while (split_cmd[j])
	{
		argv[j] = ft_strdup(split_cmd[j]);
		free(split_cmd[j]);
		j++;
	}
	argv[j] = NULL;
	free (split_cmd);
	return (argv);
}

char	*init_abs_comd(char *comd, char *path_i)
{
	char	*dir_cmd;
	char	*abs;

	dir_cmd = ft_strjoin(path_i, "/");
	abs = ft_strjoin(dir_cmd, comd);
	free(dir_cmd);
	return (abs);
}

int	get_cmd_size(char *cmd)
{
	int		j;
	char	**split_cmd;

	j = 0;
	split_cmd = ft_split(cmd, ' ');
	while (split_cmd[j])
		j++;
	free_split(split_cmd);
	return (j);
}
