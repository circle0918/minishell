#include "../includes/minishell.h"

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
	else
		return (0);
}

int launch(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	int j;
	int size;

	j = 0;
	char **split_cmd = ft_split(cmd, ' ');
	while(split_cmd[j])
	{
		j++;
	}
	size = j + 1 ;
	//printf("size %d\n", size);
	char *argv[size];
	j = 0;
	while(j < size -1)
	{
		argv[j] = ft_strdup(split_cmd[j]);
		free(split_cmd[j]);
		//printf("argv[%d]: %s\n", j, argv[j]);
		j++;
	}
	argv[j] = NULL;
	char *dir_cmd;
	if (abs_path_test == NULL)
		dir_cmd = ft_strjoin(g->path[i], "/");
	else
		dir_cmd = ft_strjoin(abs_path_test, "/");
	char *abs_cmd = ft_strjoin(dir_cmd, comd); //str_replace TODO (ls    -a = ls -a)
	//printf("abs_cmd: %s\n", abs_cmd);
	if (is_buildin(comd, cmd, g) == 0)	
	{
		if (execve(abs_cmd, argv, NULL) == -1)
			return (-1);
	}
	free(dir_cmd);
	free(abs_cmd);
	j = 0;
	while(j < size)
	{
		free (argv[j]);
		j++;
	}
	return (0);
}


int launcher(char *cmd, char *comd, t_ms *g, int i, char *abs_path_test)
{
	pid_t pid, wpid;
	int status;
	
	pid = fork();
	if (pid == 0)
	{
	  // Child process
		if (launch(cmd, comd, g, i, abs_path_test) == -1)
	  		perror("launch error");
		exit(EXIT_FAILURE);
	} 
	else if (pid < 0)
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
			wpid = waitpid(pid, &status, WUNTRACED);
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
	char			*comd;
	i = 0;

	if(ft_strequ(g->line, "\0"))
		return (1);
	if(ft_strequ(g->line, "env"))
	{
		print_list(g->env);
		return (1);
	}
	comd = get_cmd_in_line(cmd);
	if(ft_strcmp(comd, "export") == 0)
	{
		if (launch(cmd, comd, g, i, NULL) == -1)
	  		perror("launch error");
		return (1);
	}
	if(ft_strcmp(comd, "unset") == 0)
	{
		if (launch(cmd, comd, g, i, NULL) == -1)
	  		perror("launch error");
		return (1);
	}
	if(ft_strcmp(comd, "cd") == 0)
	{
		if (launch(cmd, comd, g, i, NULL) == -1)
	  		perror("launch error");
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
				if (ft_strequ(dirp->d_name, comd))
				{
					launcher(cmd, comd, g, i, NULL);
					free(comd);
					closedir(dir);
					return (1);
				}
			}
		}
		closedir(dir);
		i++;
	}
	free(comd);
	return (0);
}

