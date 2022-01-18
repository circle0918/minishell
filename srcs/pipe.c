/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 15:31:37 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 17:53:36 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execution(char *cmd, int fd_in[2], int fd_out[2], t_ms *g)
{
	g->pid[1] = fork();
	g->ret_errno = 0;
	if (g->pid[1] == -1)
		exit(EXIT_FAILURE);
	else if (g->pid[1] == 0)
	{
		if (fd_in[0] != -1 && fd_in[1] != -1)
		{
			close(fd_in[1]);
			dup2(fd_in[0], STDIN_FILENO);
			close(fd_in[0]);
		}
		if (fd_out[0] != -1 && fd_out[1] != -1 && g->last_cmd != 1)
		{
			close(fd_out[0]);
			dup2(fd_out[1], STDOUT_FILENO);
			close(fd_out[1]);
		}
		find_cmd_path(cmd, g);
		exit(g->ret_errno);
	}
}

void	preexecution(char **cmd, int fd_in[2], int fd_out[2], t_ms *g)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (i < g->nb_cmd_pipe)
			pipe(fd_out);
		execution(cmd[i], fd_in, fd_out, g);
		if (fd_in[0] != -1)
			close(fd_in[0]);
		close(fd_in[1]);
		if (i < g->nb_cmd_pipe)
		{
			fd_in[0] = fd_out[0];
			fd_in[1] = fd_out[1];
		}
		else
		{
			fd_out[0] = -1;
			fd_out[1] = -1;
		}
		if (i == g->nb_cmd_pipe - 1)
			g->last_cmd = 1;
		i++;
	}
}

static char	**norm(char **pipe_command, char *line, int nb, int j)
{
	ft_strlcpy(pipe_command[j], line, nb);
	pipe_command[j + 1] = NULL;
	g_ms->nb_cmd_pipe = j;
	return (pipe_command);
}

static char	**reccord_cmd_pipe(char **pipe_command, t_ms *g, int i, int j)
{
	int	c;
	int	first;

	first = 0;
	while (g->line[i++])
	{
		if (g->line[i] == '\'' || g->line[i] == '"')
			i = parseur_quotes(g, i + 1, g->line[i]);
		if (g->line[i] == '|')
		{
			c = move_space_before(g->line, i);
			pipe_command[j] = (char *)malloc(sizeof(char) * (c - first) + 1);
			if (!pipe_command[j])
				return (NULL);
			ft_strlcpy(pipe_command[j++], &g->line[first], c - first + 1);
			i = move_space_after(g->line, i);
			first = i + 1;
		}
		if (g->line[i] == '\\')
			i++;
	}
	pipe_command[j] = (char *)malloc(sizeof(char) * (i - first) + 1);
	if (!pipe_command[j])
		return (NULL);
	return (norm(pipe_command, &g->line[first], i - first + 1, j));
}

void	pipe_command(t_ms *g, int pipe)
{
	char	**pipe_command;

	pipe_command = (char **)malloc(sizeof(char *) * (pipe + 2));
	if (!pipe_command)
		return ;
	pipe_command = reccord_cmd_pipe(pipe_command, g, 0, 0);
	if (pipe_command == NULL)
	{
		write(2, "error: malloc failed\n", 21);
		free_split(pipe_command);
		return ;
	}
	my_pipe(pipe_command, g);
	free_split(pipe_command);
}
