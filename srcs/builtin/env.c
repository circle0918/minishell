#include "../includes/minishell.h"
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
	(void)g;

	i = 0;
	tmp = NULL;
	if (ft_strchr(cmd, '>') || ft_strchr(cmd, '<'))
	{
		while (cmd[i] && cmd[i] != '>')
			i++;
		tmp = ft_strdup(cmd + i);
		strcpy_del_c(tmp, g);
		free(tmp);
	}
	// return (tmp);
}

void	exit_free(char **str)
{
	int i;

	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
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
	return (cmd);
}
void	get_path(t_ms *g)
{
	char *path;
	char *path_tmp;
	t_list *tmp;
	char *pos;

	tmp = g->env;
	path = NULL;
	path_tmp = NULL;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, "PATH=");
		if (pos && pos - (char *)tmp->content == 0)
		{
			path_tmp = ft_strdup(tmp->content);
			break;
		}
		free(path_tmp);
		tmp = tmp->next;
	}
	path = ft_substr(path_tmp, 5, (ft_strlen(path_tmp) - 5));
	free(path_tmp);
	g->path = ft_split(path, ':');
	free(path);
}
int		is_buildin(char *comd, char *cmd, t_ms *g)
{
//	printf("comd %s\n",comd);
	if (ft_strcmp(comd, "pwd") == 0)
	{
		ft_pwd();
		return (1);
	}
	else if (ft_strcmp(comd, "echo") == 0)
	{
		ft_echo(cmd, g);
		return (1);
	}
	else if (ft_strcmp(comd, "cd") == 0)
	{
		ft_cd(comd, cmd, g);
		return (1);
	}
	else if (ft_strcmp(comd, "export") == 0)
	{
		ft_export(cmd, g);
		return (1);
	}
	else if (ft_strcmp(comd, "unset") == 0)
	{
		ft_unset(comd, cmd, g);
		return (1);
	}
	else if (ft_strcmp(comd, "env") == 0)
	{
		print_list(g->env);
		return (1);
	}
	else
		return (0);
}
void init_argv(char **argv, char *cmd)
{
	int	j;
	char **split_cmd;

	split_cmd = ft_split(cmd, ' ');
	j = 0;
	while(split_cmd[j])
	{
		argv[j] = ft_strdup(split_cmd[j]);
		free(split_cmd[j]);
		//printf("in argv[%d]: %s\n", j, argv[j]);
		j++;
	}
	argv[j] = NULL;
	free(split_cmd);
}

void init_abs_comd(char **abs_cmd, char *comd, t_ms *g, char *abs_path_test, int i)
{
	char *dir_cmd;

	if (abs_path_test == NULL)
		dir_cmd = ft_strjoin(g->path[i], "/");
	else
		dir_cmd = ft_strjoin(abs_path_test, "/");
	*abs_cmd = ft_strjoin(dir_cmd, comd);
	//printf("abs_cmd: %s\n", *abs_cmd);
	free(dir_cmd);
}

int get_cmd_size(char *cmd)
{
	int j;
	char **split_cmd;

	j = 0;
	split_cmd = ft_split(cmd, ' ');
	while(split_cmd[j])
	{
		j++;
		// free(split_cmd[j]);
	}
	free_split(split_cmd);
	return (j);
}

int launch(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	int j;

	//printf("size %d\n", size);
	char *argv[get_cmd_size(cmd) + 1];
	init_argv(argv, cmd);
	char *abs_comd;
	init_abs_comd(&abs_comd, comd, g, abs_path_test, i);
	if (is_buildin(comd, cmd, g) == 0)
	{
		if (execve(abs_comd, argv, NULL) == -1)
			return (-1);
	}
	free(abs_comd);
	j = 0;
	while(argv[j])
	{
		//printf("out argv[%d]: %s\n", j, argv[j]);
		free (argv[j]);
		j++;
	}
	return (0);
}


int launcher(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	// pid_t pid;
	pid_t wpid;
	int status;

	g_ms->pid[0] = fork();
	if (g_ms->pid[0] == 0)
	{
	  // Child process
		if (launch(cmd, comd, g, i, abs_path_test) == -1)
	  		perror("launch error");
		exit(EXIT_FAILURE);
	}
	else if (g_ms->pid[0] < 0)
	{
		// Error forking
		perror("lsh");
	}
	else
	{
		// Parent process
//		if (ft_strequ(comd, "minishell"))
//			signal(SIGINT, SIG_IGN);
		do {
			wpid = waitpid(g_ms->pid[0], &status, WUNTRACED);
/*            if (wpid == -1) {
                perror("ERROR waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) {
                printf("terminé, code=%d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("tué par le signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("arrêté par le signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("relancé\n");
            }*/
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
       // printf("Parent: child %d died with status 0x%.4X\n", pid, status);
	}
	return 1;
}
int		exec_cmd_has_dir(char *cmd, char *comd, t_ms *g, int i)
{
	int l;
	char *path;
	char *exec;
	DIR				*dir;
	struct dirent	*dirp;

	l = ft_strlen(comd) - 1;
	while(l >= 0)
	{
		if(comd[l] == '/')
			break;
		l--;
	}
	if (l == -1)
		return 0;
	path = ft_substr(comd, 0, l);
	//printf("cutting path: %s\n", path);
	//path-->abs_path
	if (path[0] != '/')
	{
		char *tmp = path;
		char *tmp2 = ft_strjoin(get_env("PWD", g->env), "/");
		path = ft_strjoin(tmp2, tmp);
		free(tmp);
		free(tmp2);
	}
	exec = ft_substr(comd, l + 1, ft_strlen(comd) - l);
	//printf("cutting exec: %s\n", exec);
	dir = opendir(path);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, exec))
				{
					launcher(cmd, exec, g, i, path);
					free(comd);
					free(path);
					free(exec);
					closedir(dir);
					return (1);
				}
			}
		}
		closedir(dir);
		free(comd);
	return (0);
}

int		find_cmd_path(char *cmd, t_ms *g)
{
	DIR				*dir;
	struct dirent	*dirp;
	int				i;
	// char			*comd;
	// char			*cmd_tmp;
	char			**master_cmd;

	i = 0;
	if (ft_strchr(cmd, '$'))
	{
		cmd = check_var_cmd(g, cmd);
		if (!cmd || ft_strequ(cmd, "\0"))
		{
			// free(cmd);
			return (1);
		}
	}
	master_cmd = creat_list_arg(cmd);
	test_redir_flag(cmd, g);
	// comd = get_cmd_in_line(cmd);
	// print_split(master_cmd);
	if(g->ret_dir)
	{
	//S	go_redir(comd, g->ret_dir);
	}
	if (!ft_strcmp(master_cmd[0], "exit"))
	{
		ft_exit_plus(master_cmd);
		free_split(master_cmd);
		g->exit = 1;
		ft_exit(2, g, g->ret, g->line);
	}
	if(ft_strcmp(master_cmd[0], "export") == 0)
	{
		if (launch(cmd, master_cmd[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(master_cmd);
		// free(cmd);
		return (1);
	}
	if(ft_strcmp(master_cmd[0], "unset") == 0)
	{
		if (launch(cmd, master_cmd[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(master_cmd);
		// free(cmd);
		return (1);
	}
	if(ft_strcmp(master_cmd[0], "cd") == 0)
	{
		if (launch(cmd, master_cmd[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(master_cmd);
		// free(cmd);
		return (1);
	}
//	if (exec_cmd_has_dir(cmd, comd, g, i) == 1)
//		return (1);
	while (g->path[i])
	{
		dir = opendir(g->path[i]);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, master_cmd[0]))
				{
					launcher(cmd, master_cmd[0], g, i, NULL);
					// free(comd);
					closedir(dir);
					free_split(master_cmd);
					// free(cmd);
					return (1);
				}
			}
		}
		closedir(dir);
		i++;
	}
	// free(comd);
	// free(cmd);
	free_split(master_cmd);
	return (0);
}

