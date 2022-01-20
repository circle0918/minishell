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

char	**get_argv(t_ms *g, char *cmd)
{
	char	**argv;

	argv = init_argv(cmd);
	if (g->ret_dir)
	{
		free_split(argv);
		argv = get_argv_redir(cmd);
	}
	return (argv);
}

int	do_redir(t_ms *g, char *cmd, int *out, int *in)
{
	if (g->ret_dir)
	{
		*in = get_redir_in_file(cmd);
		if (*in == -1)
			return (-1);
		*out = get_redir_out_file(cmd);
		if (*out > 0)
		{
			if (dup2(*out, STDOUT_FILENO) == -1)
				perror("Error redir out");
		}
		if (*in > 0)
		{
			if (dup2(*in, STDIN_FILENO) == -1)
				perror("Error redir in");
			close(*in);
		}
	}
	return (0);
}

void	clean_redir(int *out, int *in)
{
	if (*out > 0)
		close(*out);
	if (*in > 0)
		close(*in);
	unlink("redir_lessless");
}

int	launch_exec(char *cmd, char *comd, t_ms *g, char *path_i)
{	
	char	**argv;
	char	*abs_comd;
	char	**env;

	argv = get_argv(g, cmd);
	abs_comd = init_abs_comd(comd, path_i);
	env = get_env_tab(g->env);
	if (execve(abs_comd, argv, env) == -1)
	{
		free(abs_comd);
		free_split(argv);
		free_split(env);
		return (-1);
	}
	free(abs_comd);
	free_split(argv);
	free_split(env);
	return (0);
}

int	launch(char *cmd, char *comd, t_ms *g, char *path_i)
{
	int	try_buildin;

	if (do_redir(g, cmd, &g->redir_out_fd, &g->redir_in_fd) == -1)
		return (-1);
	try_buildin = is_buildin(comd, g);
	if (try_buildin == -1)
	{
		error_out2(comd, NULL, "No such file or directory");
		g->ret_errno = 127;
	}
	else if (try_buildin == 0)
	{
		if (launch_exec(cmd, comd, g, path_i) == -1)
			return (-1);
	}
	return (0);
}
