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

char	*find_cmd_in_path_i(char *cmd, char *path_i);

int	count_space(char *tmp)
{
	int	i;
	int	x;

	i = -1;
	x = 0;
	while (tmp[++i])
	{
		if (tmp[i] == ' ')
		x++;
	}
	return (x);
}

void	strcpy_del_c(char *tmp, t_ms *g)
{
	int	x;
	int	i;

	x = 0;
	i = 0;
	g->ret_dir = malloc(sizeof(char) * (ft_strlen(tmp) - count_space(tmp) + 1));
	if (!g->ret_dir)
		return ;
	while (tmp[i])
	{
		if (tmp[i] != ' ')
			g->ret_dir[x++] = tmp[i++];
		else
			i++;
	}
	g->ret_dir[x] = 0;
}

void	test_redir_flag(char *cmd, t_ms *g)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (ft_strchr(cmd, '>') || ft_strchr(cmd, '<'))
	{
		while (cmd[i] && cmd[i] != '>' && cmd[i] != '<')
			i++;
		tmp = ft_strdup(cmd + i);
		strcpy_del_c(tmp, g);
		free(tmp);
	}
}

char	*get_cmd_in_line(char *line)
{
	char	*cmd;
	int		i;
	int		pos;

	i = 0;
	pos = 0;
	cmd = 0;
	while (ft_isspace(line[i]) == 1)
	{
		pos = i;
		i++;
	}
	if (i != 0)
		pos++;
	while (line[i] && ft_isspace(line[i]) == 0)
	{
		i++;
	}
	cmd = ft_substr(line, pos, i - pos);
	return (cmd);
}

void	get_path(t_ms *g)
{
	char	*path;

	path = NULL;
	path = ft_strdup(get_env("PATH", g->env));
	g->path = ft_split(path, ':');
	free(path);
}
