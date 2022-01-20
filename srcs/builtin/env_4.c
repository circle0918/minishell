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

int	launcher(char *cmd, char *comd, t_ms *g, char *path_i)
{
	int	status;

	g_ms->pid[0] = fork();
	if (g_ms->pid[0] == 0)
	{
		if (launch(cmd, comd, g, path_i) == -1)
			perror("Error fork launch");
		exit(EXIT_FAILURE);
	}
	else if (g_ms->pid[0] < 0)
		perror("Error forking");
	else
	{
		while (1)
		{
			if (waitpid(g_ms->pid[0], &status, WUNTRACED) == -1)
			{
				exit (EXIT_FAILURE);
			}
			if (WIFEXITED(status) || WIFSIGNALED(status))
				break ;
		}
		clean_redir(&g->redir_out_fd, &g->redir_in_fd);
	}
	return (1);
}

char	*get_abs_path(int pos, char *comd)
{
	char	*path;
	char	*tmp;
	char	*tmp2;

	path = ft_substr(comd, 0, pos);
	if (path[0] != '/')
	{
		tmp = path;
		tmp2 = ft_strjoin(getenv("PWD"), "/");
		path = ft_strjoin(tmp2, tmp);
		free(tmp);
		free(tmp2);
	}
	return (path);
}

int	get_last_char_pos(char *s, char c)
{
	int	l;

	l = ft_strlen(s) - 1;
	while (l >= 0)
	{
		if (s[l] == c)
			break ;
		l--;
	}
	if (l == -1)
		return (-1);
	return (l);
}

int	exec_cmd_has_dir(char *cmd, char *comd, t_ms *g)
{
	int		l;
	char	*path;
	char	*exec;
	char	*path_i;

	l = get_last_char_pos(comd, '/');
	if (l == -1)
		return (0);
	path = get_abs_path(l, comd);
	exec = ft_substr(comd, l + 1, ft_strlen(comd) - l);
	path_i = find_cmd_in_path_i(exec, path);
	if (path_i)
	{
		launcher(cmd, exec, g, path);
		free(path);
		free(exec);
		return (1);
	}
	return (0);
}

void	init_redir(t_ms *g, char *cmd)
{
	if (g->ret_dir)
	{
		free_split(g->cmd_tab);
		g->cmd_tab = get_argv_redir(cmd);
	}
}
