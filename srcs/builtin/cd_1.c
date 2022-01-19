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

void	error_out2(char *comd, char *opt, char *msg)
{
	char	str[1024];

	ft_memset(str, 0, 1024);
	ft_strcat(str, "minishell : ");
	if (comd)
	{
		ft_strcat(str, comd);
		ft_strcat(str, ": ");
	}
	if (opt)
	{
		ft_strcat(str, opt);
		ft_strcat(str, ": ");
	}
	ft_strcat(str, msg);
	ft_strcat(str, "\n");
	ft_putstr_fd(str, 2);
}

char	*get_env(char *str, t_list *env)
{
	t_list	*tmp;
	char	*pos;

	tmp = env;
	pos = NULL;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, str);
		if (pos && pos - (char *)tmp->content == 0)
		{
			return (pos + ft_strlen(str) + 1);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	set_env_replace(char *key, t_list *env, char (*s)[])
{
	t_list	*tmp;
	char	*pos;

	pos = NULL;
	tmp = env;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, key);
		if (pos && pos - (char *)tmp->content == 0)
		{
			free(tmp->content);
			tmp->content = ft_strdup(*s);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	set_env(char *key, char *val, t_list *env)
{
	char	s[1024];
	t_list	*last;

	ft_memset(s, 0, 1024);
	ft_strcat(s, key);
	ft_strcat(s, "=");
	ft_strcat(s, val);
	if (set_env_replace(key, env, &s) == 1)
		return ;
	last = ft_lst_pop_last(&env);
	record_list(&env, s);
	ft_lstadd_back(&env, last);
}

void	change_path(t_ms *g, int change_back)
{
	char	*pwd;
	char	path[1024];

	ft_memset(path, 0, 1024);
	getcwd(path, sizeof(path));
	if (change_back)
	{
		pwd = ft_strdup(get_env("PWD", g->env));
		set_env("PWD", path, g->env);
		set_env("OLDPWD", pwd, g->env);
		free(pwd);
	}
	else
	{
		pwd = get_env("PWD", g->env);
		set_env("OLDPWD", pwd, g->env);
		set_env("PWD", path, g->env);
	}
}
