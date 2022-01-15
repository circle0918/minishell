/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:33:33 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/15 20:57:34 by thhusser         ###   ########.fr       */
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
}

void	ft_exit(int nb, t_ms *g)
{
	if (nb == 2)
		ft_putstr("exit\n");
	// ft_del_line(g->line); --> re check si a enlever !
	ft_del_line(g->ret_dir);
	ft_lstclear(&g->env, &ft_del_list);
	ft_lstclear(&g->cmd, &ft_del_list);
	ft_lstclear(&g->cmd_tmp, &ft_del_list);
	ft_lstclear(&g->error, &ft_del_list);
	free_split(g->path);
	exit(errno);
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
				errno = 1;
				return ;
			}
			res = ft_atoi(cmd[1]);
			errno = res;
			return ;
		}
		else if (cmd[1] != NULL)
		{
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			errno = 2;
			return ;
		}
		errno = 0;
	}
}
