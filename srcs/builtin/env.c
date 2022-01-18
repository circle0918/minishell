#include "../includes/minishell.h"
char *find_cmd_in_path_i(char *cmd, char *path_i);
int     count_space(char *tmp)
{
    int i;
    int x;

    i = -1;
    x = 0;
    while (tmp[++i])
    {
        if (tmp[i] == ' ')
            x++;
    }
    return (x);
}

void    strcpy_del_c(char *tmp, t_ms *g)
{
    int x;
    int i;

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
	int i;
	char *tmp;

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
	char *cmd;
	int i;
	int pos;
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
	while(line[i] && ft_isspace(line[i]) == 0)
	{
		i++;
	}
	cmd = ft_substr(line, pos, i-pos);
	// free(line);
	return (cmd);
}

void	get_path(t_ms *g)
{
	char *path;

	path = NULL;
	path = ft_strdup(get_env("PATH", g->env));
	g->path = ft_split(path, ':');
	free(path);
}

int		is_buildin_2(char *comd, char *cmd, t_ms *g)
{
	if (ft_strcmp(comd, "export") == 0)
	{
		ft_export(cmd, g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "unset") == 0)
	{
		ft_unset(g);
		return (1);
	}
	if (g->unset_path == 1)
		return -1;
	if (ft_strcmp(comd, "env") == 0)
	{
		print_list(g->env);
		return (1);
	}
	else
		return (0);
}

int		is_buildin(char *comd, char *cmd, t_ms *g)
{
	if (ft_strcmp(comd, "pwd") == 0)
	{
		ft_pwd(g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "echo") == 0)
	{
		ft_echo(g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "cd") == 0)
	{
		ft_cd(g);
		return (1);
	}
	else
		return (is_buildin_2(comd, cmd, g));
}
char **init_argv(char *cmd)
{
	int	j;
	char **split_cmd;
	char **argv;

	argv = (char **)malloc(sizeof(char *) * (get_cmd_size(cmd) + 1));
	split_cmd = ft_split(cmd, ' ');
	j = 0;
	while(split_cmd[j])
	{
		argv[j] = ft_strdup(split_cmd[j]);
		free(split_cmd[j]);
		j++;
	}
	argv[j] = NULL;
	free(split_cmd);
	return (argv);
}

char *init_abs_comd(char *comd, char *path_i, char *abs_path_test)
{
	char *dir_cmd;
	char *abs;

	if (abs_path_test == NULL)
		dir_cmd = ft_strjoin(path_i, "/");
	else
		dir_cmd = ft_strjoin(abs_path_test, "/");
	abs = ft_strjoin(dir_cmd, comd);
	free(dir_cmd);
	return (abs);
}

int get_cmd_size(char *cmd)
{
	int j;
	char **split_cmd;

	j = 0;
	split_cmd = ft_split(cmd, ' ');
	while(split_cmd[j])
		j++;
	free_split(split_cmd);
	return (j);
}

char **get_argv(t_ms *g, char *cmd)
{
	char **argv;

	argv = init_argv(cmd);
	if (g->ret_dir)
	{
		free_split(argv);
		argv = get_argv_redir(cmd);
	}
	return argv;
}

int do_redir(t_ms *g, char *cmd)
{
	int	redir_out_fd;
	int	redir_in_fd;

	if (g->ret_dir)
	{
		redir_in_fd = get_redir_in_file(cmd);
		if (redir_in_fd == -1)
	      		return (-1);
		redir_out_fd = get_redir_out_file(cmd);
        	if (redir_out_fd > 0)
		{
	    		if (dup2(redir_out_fd, STDOUT_FILENO) == -1)
				perror("Error redir out");
		}
		if (redir_in_fd > 0)
		{
	    		if (dup2(redir_in_fd, STDIN_FILENO) == -1)
				perror("Error redir in");
			close(redir_in_fd);
		}
	}
	return (0);
}

int launch(char *cmd, char *comd, t_ms *g, char *path_i, char *abs_path_test)
{
	char **argv;
	char *abs_comd;

	argv = get_argv(g, cmd);
	abs_comd = init_abs_comd(comd, path_i, abs_path_test);
	if (do_redir(g, cmd) == -1)
		return (-1);
	char **env;
	env = NULL;
	//printf("before exec: abs_comd: %s\n", abs_comd);
	//print_2Dtab(argv, "before exec: argv");

	int ret = is_buildin(comd, cmd, g);
	if (ret == -1)
	{
		error_out2(comd, NULL, "No such file or directory");
		g->ret_errno = 127;
//		return (1);
	}
	else if (ret == 0)
	{
		//printf("b exec==============\n");
		env = get_env_tab(g->env);
		if (execve(abs_comd, argv, env) == -1) {
			free(abs_comd);
			free_split(argv);
			free_split(env);
			return (-1);
		}
	}
	free(abs_comd);
	free_split(argv);
	free_split(env);
	return (0);
}


int launcher(char *cmd, char *comd, t_ms *g, char *path_i, char *abs_path_test)
{
	int	status;

	g_ms->pid[0] = fork();
	if (g_ms->pid[0] == 0)
	{
		if (launch(cmd, comd, g, path_i, abs_path_test) == -1)
	 		perror("Error fork launch");
		// {
			// ft_exit(0, g, 0, NULL);
		exit(EXIT_FAILURE);
		// }
		// ft_exit(0, g, 0, NULL);
		// exit(0);
	}
	else if (g_ms->pid[0] < 0)
		perror("Error forking");
	else
	{
		while (1)
		{
			if(waitpid(g_ms->pid[0], &status, WUNTRACED) == -1)
		      	{
                		perror("ERROR waitpid");
                		exit(EXIT_FAILURE);
            		}
	    		if (WIFEXITED(status)) {
                	//	printf("terminé, code=%d\n", WEXITSTATUS(status));
				g->ret_errno = WEXITSTATUS(status);
            		}
		/*	else if (WIFSIGNALED(status)) {
                		printf("tué par le signal %d\n", WTERMSIG(status));
            		} else if (WIFSTOPPED(status)) {
                		printf("arrêté par le signal %d\n", WSTOPSIG(status));
            		} else if (WIFCONTINUED(status)) {
                		printf("relancé\n");
            		}
		*/	if (WIFEXITED(status) || WIFSIGNALED(status))
				break;
		}
	}
	return (1);
}

char *get_abs_path(int pos, char *comd)
{
	char *path;
	char *tmp;
	char *tmp2;

	path = ft_substr(comd, 0, pos);
	//printf("cutting path: %s\n", path);
	if (path[0] != '/')
	{
		tmp = path;
		tmp2 = ft_strjoin(getenv("PWD"), "/");
		path = ft_strjoin(tmp2, tmp);
	//	printf("changing path: %s\n", path);
		free(tmp);
		free(tmp2);
	}
	return (path);
}

int get_last_char_pos(char *s, char c)
{
	int	l;

	l = ft_strlen(s) - 1;
	while(l >= 0)
	{
		if(s[l] == c)
			break;
		l--;
	}
	if (l == -1)
		return (-1);
	return (l);
}

int		exec_cmd_has_dir(char *cmd, char *comd, t_ms *g, char *dir)
{
	int	l;
	char	*path;
	char	*exec;

	l = get_last_char_pos(comd, '/');
	if (l == -1)
		return 0;
	path = get_abs_path(l, comd);
	exec = ft_substr(comd, l + 1, ft_strlen(comd) - l);
	//printf("cutting exec: %s\n", exec);
	char *path_i = find_cmd_in_path_i(exec, path);
	if (path_i)
	{
		launcher(cmd, exec, g, dir, path);
		free(path);
		free(exec);
		return (1);
	}
	return (0);
}

int		count_tab(char **tab)
{
	int i;

	i = 0;
	while(tab[i])
	{
		i++;
	}
	return (i);
}

void init_redir(t_ms *g, char *cmd)
{
	if (g->ret_dir)
	{
		free_split(g->cmd_tab);
		g->cmd_tab = get_argv_redir(cmd);
	}
}

char *find_cmd_in_path_i(char *cmd, char *path_i)
{
	DIR	*dir;
	struct dirent	*dirp;
	char	*ret;

	dir = opendir(path_i);
	if (dir)
	{
		while ((dirp = readdir(dir)) != NULL)
		{
			if (ft_strequ(dirp->d_name, cmd))
			{
				ret = path_i;
				closedir(dir);
				return (ret);
			}
		}
	}
	closedir(dir);
	return (NULL);
}

char *find_cmd_in_path_tab(t_ms *g)
{
	int	i;
	char	*path_i;

	i = 0;
	while (g->path[i])
	{
		path_i = find_cmd_in_path_i(g->cmd_tab[0], g->path[i]);
		if (path_i)
			return (path_i);
		i++;
	}
	return (NULL);
}

int handle_cmd_noneed_fork(t_ms *g, char *cmd)
{
	if (!ft_strcmp(g->cmd_tab[0], "exit"))
	{
		ft_exit_plus(g->cmd_tab);
		free_split(g->cmd_tab);
		g->exit = 1;
		ft_exit(2, g, g->ret, g->line);
	}
	if(ft_strcmp(g->cmd_tab[0], "export") == 0
		|| ft_strcmp(g->cmd_tab[0], "unset") == 0
		|| ft_strcmp(g->cmd_tab[0], "cd") == 0)
	{
		g->ret_errno = 0;
		if (launch(cmd, g->cmd_tab[0], g, g->path[0], NULL) == -1)
	  		perror("launch error");
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}

int		find_cmd_path(char *cmd, t_ms *g)
{
	if (ft_strchr(cmd, '$'))
	{
		cmd = check_var_cmd(g, cmd);
		if (!cmd || ft_strequ(cmd, "\0"))
		{
			free_split(g->cmd_tab);
			return (1);
		}
	}
	g->cmd_tab = creat_list_arg(cmd);
	g->cmd_ac = count_tab(g->cmd_tab);
	test_redir_flag(cmd, g);
	init_redir(g, cmd);
	if (handle_cmd_noneed_fork(g, cmd) == 1)
		return (1);
	if (exec_cmd_has_dir(cmd, g->cmd_tab[0], g, g->path[0]) == 1)
	{
		free_split(g->cmd_tab);
		return (1);
	}

	char *path_i;
	path_i = find_cmd_in_path_tab(g);
	if (path_i)
	{
		launcher(cmd, g->cmd_tab[0], g, path_i, NULL);
		free_split(g->cmd_tab);
		return (1);
	}
	return (0);
}
