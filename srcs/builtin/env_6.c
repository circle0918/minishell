/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 16:05:45 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 16:06:38 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*test_redir_flag_2(char *cmd)
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
		return (tmp);
	}
	return (NULL);
}

char	*norm_tmp_cote(t_ms *g, char *tmp, char *str)
{
	if (g->tmp)
	{
		tmp = ft_strjoin(str, " ");
		ft_del_line(str);
		str = ft_strjoin(tmp, g->tmp);
		ft_del_line(tmp);
		ft_del_line(g->tmp);
	}
	return (str);
}

char	*norm_clean_line_cmd_2(t_ms *g, int count, char *str, char *tmp)
{
	int	i;

	i = -1;
	while (g->cmd_tab[++i])
	{
		if (i == 0)
			str = ft_strjoin(g->cmd_tab[i], "");
		else if (count - 1 == i)
		{
			tmp = ft_strjoin(str, " ");
			ft_del_line(str);
			str = ft_strjoin(tmp, g->cmd_tab[i]);
			ft_del_line(tmp);
			break ;
		}
		else if (i != 0)
		{
			tmp = ft_strjoin(str, " ");
			ft_del_line(str);
			str = ft_strjoin(tmp, g->cmd_tab[i]);
			ft_del_line(tmp);
		}
	}
	str = norm_tmp_cote(g, tmp, str);
	return (str);
}
