/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:33:33 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/18 03:07:29 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_split(char **split)
{
	int i;

	i = -1;
	if (split)
	{
		while (split[++i])
				free(split[i]);
		free(split);
	}
	split = NULL;
}

void	ft_exit(int nb, t_ms *g, int ret, char *line)
{
	if ((!ret && line[0] == '\0') || g->exit)
	{
		if (nb == 2)
			ft_putstr("exit\n");
		rl_clear_history();
		ft_del_line(g->line); //--> re check si a enlever !
		ft_del_line(g->ret_dir);
		ft_lstclear(&g->env, &ft_del_list);
		ft_lstclear(&g->cmd, &ft_del_list);
		ft_lstclear(&g->cmd_tmp, &ft_del_list);
		ft_lstclear(&g->error, &ft_del_list);
		free_split(g->path);
		exit(g->ret_errno);
	}
	if ((!ret && line[0] != '\0'))
	{
		ft_putstr("  \b\b \b");
		// write(0, "\n", 1);
	}

}

int		ft_strisdigit(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void		ft_exit_plus(char **cmd)
{
	unsigned long long res;

	res = 0;
	if (cmd != NULL)
	{
		if (cmd[1] != NULL && ft_strisdigit(cmd[1]))
		{
			if (cmd[2] != NULL)
			{
				write(2, "bash: exit: too many arguments\n", 31);
				g_ms->ret_errno = 1;
				return ;
			}
			res = ft_atoi(cmd[1]);
			g_ms->ret_errno = res;
			return ;
		}
		else if (cmd[1] != NULL)
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_ms->ret_errno = 2;
			return ;
		}
		// g_ms->ret_errno = 0;
	}
}
