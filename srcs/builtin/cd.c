#include "../includes/minishell.h"

void	error_out2(char *comd, char *opt, char *msg)
{
	char	str[1024];

	str[0] = '\0';
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

int set_env_replace(char *key, t_list *env, char* s)
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
			tmp->content = ft_strdup(s);
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

	s[0] = '\0';
	ft_strcat(s, key);
	ft_strcat(s, "=");
	ft_strcat(s, val);
	if (set_env_replace(key, env, s) == 1)
		return ;
	last = ft_lst_pop_last(&env);
	record_list(&env, s);
	ft_lstadd_back(&env, last);
}

void	change_path(char *path, t_ms *g, int change_back)
{
	char	*pwd;

	path = get_pwd();
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

int ft_cd_0(t_ms *g, char **path)
{
	if (g->cmd_ac == 2)
		*path = ft_strdup(g->cmd_tab[1]);
	g->ret_errno = 0;
	if (g->cmd_ac > 2)
	{
		g->ret_errno = 1;
		error_out2("cd", *path, "Too many arguements");
		return (1);	
	}
	if (g->cmd_ac == 1 || ft_strequ(*path, "~") || ft_strequ(*path, "--"))
	{
		if (g->cmd_ac == 2)
			free(*path);
		*path = get_env("HOME", g->env);
	}
	return (0);
}
int ft_cd_1(t_ms *g, char **path)
{
	char	*tmp_path;
	
	if (ft_strequ(*path, "-"))
	{
		free(*path);
		*path = get_env("OLDPWD", g->env);
		if (chdir(*path) == 0)
			change_path(*path, g, 1);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", *path, "No such file or directory");
		}
		return (1);
	}
	if ((*path)[0] == '~')
	{
		tmp_path = *path;
		*path = ft_strjoin(get_env("HOME", g->env), (tmp_path + 1));
		free(tmp_path);
	}
	return (0);
}

void ft_cd_2(t_ms *g, char **path)
{
	char	cwd[1024];
	
	getcwd(cwd, sizeof (cwd));
	ft_strcat(cwd, "/");
	ft_strcat(cwd, *path);
	if (chdir(cwd) == 0)
		change_path(cwd, g, 0);
	else
	{
		g->ret_errno = 1;
		error_out2("cd", *path, "No such file or directory");
		g->ret_errno = 1;
	}
}

void	ft_cd(t_ms *g)
{
	char	*path;

	path = NULL;
	if (ft_cd_0(g, &path) == 1)
		return ;
	if (ft_cd_1(g, &path) == 1)
		return ;
	if (path[0] != '/')
		ft_cd_2(g, &path);
	else
	{
		if (chdir(path) == 0)
			change_path(path, g, 0);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", path, "No such file or directory");
			g->ret_errno = 1;
		}
	}
}
