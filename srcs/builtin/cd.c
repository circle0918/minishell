#include "../includes/minishell.h"

void error_out2(char *comd, char *opt, char *msg)
{
	char str[1024];

	str[0] = '\0';
	ft_strcat(str, "minishell : ");
	if (comd)
	{
		ft_strcat(str, comd);
		ft_strcat(str, ": ");
	}
	ft_strcat(str, opt);
	ft_strcat(str, ": ");
	ft_strcat(str, msg);
	ft_strcat(str, "\n");
	ft_putstr_fd(str, 2);
}
char* get_env(char *str, t_list *env)
{
	t_list *tmp;
	char* pos;

	tmp = env;
	pos = NULL;
	while(tmp)
	{
		pos = ft_strstr(tmp->content, str);
		if (pos && pos - (char *)tmp->content == 0)
		{
			return pos + ft_strlen(str) + 1;
		}
		tmp = tmp->next;
	}
	return NULL;
}
void set_env(char *key, char *val, t_list *env)
{
	t_list *tmp;
	char* pos;
	char s[1024];
	t_list *last;


	s[0] = '\0';
	ft_strcat(s, key);
	ft_strcat(s, "=");
    ft_strcat(s, val);
	tmp = env;
	pos = NULL;
	while(tmp)
	{
		pos = ft_strstr(tmp->content, key);
		if (pos && pos - (char *)tmp->content == 0)
		{
			free(tmp->content);
			tmp->content = ft_strdup(s);
			return ;
		}
		tmp = tmp->next;
	}
	//append

	//record the new before the last(_=./minishell)
	last = ft_lst_pop_last(&env);
	record_list(&env, s);
	ft_lstadd_back(&env, last);
}
void change_path(char *path, t_ms *g, int change_back)
{
	char* pwd;

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
void ft_cd(t_ms *g)
{
	char *path;
	int only_cd;
	only_cd = 0;
	printf("cd_ac ; %d\n", g->cmd_ac);
	if(g->cmd_ac >= 2)
		path = ft_strdup(g->cmd_tab[1]);

	char cwd[1024];
	char *tmp_path;
	g->ret_errno = 0;
	if(g->cmd_ac > 2)
	{
		g->ret_errno = 1;
		error_out2("cd", path, "Too many arguements");
		free(path);
	}
	if (g->cmd_ac == 1|| ft_strequ(path, "~") || ft_strequ(path, "--"))
	{
		//path == home
		free(path);
		path = get_env("HOME", g->env);
	}
	if (ft_strequ(path, "-"))
	{
		//path == OLDPWD
		free(path);
		path = get_env("OLDPWD", g->env);
		printf("cd - :%s\n", path);
		if (chdir(path) == 0)
			change_path(path, g, 1);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", path, "No such file or directory");
		}
		return ;
	}
	if(path[0] == '~')
	{
		//path: replace ~ to home
		tmp_path = path;
		path = ft_strjoin(get_env("HOME", g->env), (tmp_path + 1));
		free(tmp_path);
	}
    if (path[0] != '/')
    {// true for the dir in cwd
        getcwd(cwd, sizeof(cwd));
        ft_strcat(cwd, "/");
        ft_strcat(cwd, path);
        if (chdir(cwd) == 0)
			change_path(cwd, g, 0);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", path, "No such file or directory");
		}
		
    }else{
        if (chdir(path) == 0)
			change_path(path, g, 0);
		else
		{
			g->ret_errno = 1;
			error_out2("cd", path, "No such file or directory");
		}
    }
}
