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
	// return (tmp);
}

void	exit_free(char **str)
{
	int i;
	
	if (!str)
		return ;
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
	if (ft_strcmp(comd, "pwd") == 0)
	{
		ft_pwd();
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
	else if (ft_strcmp(comd, "export") == 0)
	{
		ft_export(cmd, g);
		return (1);
	}
	else if (ft_strcmp(g->cmd_tab[0], "unset") == 0)
	{
		ft_unset(g);
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
		//printf("in argv[%d]: %s\n", j, argv[j]);
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
	{
		j++;
		// free(split_cmd[j]);
	}
	free_split(split_cmd);
	return (j);
}

int launch(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	char **argv;
	char *abs_comd;

	argv = init_argv(cmd);
	abs_comd = init_abs_comd(comd, g->path[i], abs_path_test);

	int redir_out_fd;
	int redir_in_fd;

	redir_out_fd = 0;
	redir_in_fd = 0;
	if (g->ret_dir)// > <
	{

		redir_in_fd = get_redir_in_file(cmd);
		if (redir_in_fd == -1)
	      		return (1);
		printf("get_redir_in fd: %d\n", redir_in_fd);
		redir_out_fd = get_redir_out_file(cmd);
		printf("get_redir_out fd: %d\n", redir_out_fd);
        	if (redir_out_fd > 0)
		{
	    		if (dup2(redir_out_fd, STDOUT_FILENO) == -1)
				perror("Error redir out");
		//	close(redir_out_fd);
		}
		if (redir_in_fd > 0)
		{
	//		close(0);
		/*	int _fd = open("redir_lessless", O_RDONLY);//TODO:unlink file
	    		if (dup2(_fd, STDIN_FILENO) == -1)
				perror("Error redir in");
			printf("_in fd after dup2: %d\n", _fd);
	    	*/	if (dup2(redir_in_fd, STDIN_FILENO) == -1)
				perror("Error redir in");
			close(redir_in_fd);
			printf("get_redir_in fd after dup2: %d\n", redir_in_fd);
		}
		exit_free(argv);
		argv = get_argv_redir(cmd);
	}
	char **env;
	printf("before exec: abs_comd: %s\n", abs_comd);
	print_2Dtab(argv, "before exec: argv");
	if (is_buildin(comd, cmd, g) == 0)
	{
		printf("b exec==============\n");
		env = get_env_tab(g->env);
		if (execve(abs_comd, argv, env) == -1) {
			free(abs_comd);
			exit_free(argv);
			exit_free(env);
			return (-1);
		}
		printf("e exec==============\n");
	}
	free(abs_comd);
	exit_free(argv);
	exit_free(env);

/*	if (redir_out_fd)
		close(redir_out_fd);
	if (redir_in_fd)
		close(redir_in_fd);
*/	return (0);
}


int launcher(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	// pid_t pid;
	pid_t wpid;
	int status;

	g_ms->pid[0] = fork();
	if (g_ms->pid[0] == 0)
	{
		if (launch(cmd, comd, g, i, abs_path_test) == -1)
	  		perror("Error fork launch");
		//exit(EXIT_FAILURE);
	}
	else if (g_ms->pid[0] < 0)
		perror("Error forking");
	else
	{
		while (1)
		{
			if (ft_strequ(comd, "minishell"))
				signal(SIGINT, SIG_IGN);
			wpid = waitpid(g_ms->pid[0], &status, WUNTRACED);
		      	if (wpid == -1) {
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
            		}
			if (WIFEXITED(status) || WIFSIGNALED(status))
				break;
		}
	}
	return 1;
}
int		exec_cmd_has_dir(char *cmd, char *comd, t_ms *g, int i)
{
	int	l;
	char	*path;
	char	*exec;
	DIR	*dir;
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
	printf("cutting path: %s\n", path);
	//path-->abs_path
	if (path[0] != '/')
	{
		char *tmp = path;
		char *tmp2 = ft_strjoin(getenv("PWD"), "/");
		path = ft_strjoin(tmp2, tmp);
		printf("changing path: %s\n", path);
		free(tmp);
		free(tmp2);
	}
	exec = ft_substr(comd, l + 1, ft_strlen(comd) - l);
	printf("cutting exec: %s\n", exec);
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



int		find_cmd_path(char *cmd, t_ms *g)
{
	DIR				*dir;
	struct dirent	*dirp;
	int				i;
	char			*comd;

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
	g->cmd_tab = creat_list_arg(cmd);

	test_redir_flag(cmd, g);
	// comd = get_cmd_in_line(cmd);
	// print_split(master_cmd);
	if (g->ret_dir)
	{
		exit_free(g->cmd_tab);
		g->cmd_tab = get_argv_redir(cmd);
	}

	g->cmd_ac = count_tab(g->cmd_tab);
	//printf("cd_ac ; %d\n", g->cmd_ac);
	//print_2Dtab(g->cmd_tab, "www");

	perror("bol 00");
	comd = get_cmd_in_line(cmd);
	if (!ft_strcmp(g->cmd_tab[0], "exit"))
	{
		ft_exit_plus(g->cmd_tab);
		free_split(g->cmd_tab);
		g->exit = 1;
		ft_exit(2, g, g->ret, g->line);
	}
	if(ft_strcmp(g->cmd_tab[0], "export") == 0)
	{
		if (launch(cmd, g->cmd_tab[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(g->cmd_tab);
		// free(cmd);
		return (1);
	}
	if(ft_strcmp(g->cmd_tab[0], "unset") == 0)
	{
		if (launch(cmd, g->cmd_tab[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(g->cmd_tab);
		// free(cmd);
		return (1);
	}
	if(ft_strcmp(g->cmd_tab[0], "cd") == 0)
	{
		if (launch(cmd, g->cmd_tab[0], g, i, NULL) == -1)
	  		perror("launch error");
		free_split(g->cmd_tab);
		// free(cmd);
		return (1);
	}
	if (exec_cmd_has_dir(cmd, comd, g, i) == 1)
		return (1);
	char *path_from_env = get_env("PATH", g->env);
	printf("path: %s\n", path_from_env);
	if (path_from_env == NULL)
	{
		error_out2(comd, NULL, "No such file or directory");
		g->ret_errno = 127;
		return (1);
	}
	while (g->path[i])
	{
		dir = opendir(g->path[i]);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, g->cmd_tab[0]))
				{
					launcher(cmd, g->cmd_tab[0], g, i, NULL);
					// free(comd);
					closedir(dir);
					free_split(g->cmd_tab);
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
	// free_split(g->cmd_tab);
	return (0);
}

