#include "../includes/minishell.h"
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
	while(ft_isspace(line[i]) == 0)
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
void	ft_pwd()
{
	ft_putstr("pwd here\n");
}
int		is_buildin(char *comd)
{
	if (ft_strcmp(comd, "pwd") == 0)
		return (1);
	else
		return (0);
}

int launch(char *cmd, char *comd, t_ms *g, int i)
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
	//	printf("%s\n", argv[j]);
		j++;
	}
	argv[j] = NULL;
	char *dir_cmd = ft_strjoin(g->path[i], "/");
	char *abs_cmd = ft_strjoin(dir_cmd, comd); //str_replace TODO (ls    -a = ls -a)
/*	ft_putstr(" cmd : ");
	ft_putstr(cmd);
	ft_putstr(" comd : ");
	ft_putstr(comd);
	ft_putstr(" toprint a0: ");
	ft_putstr(argv[0]);
	ft_putstr(" a1: ");
	ft_putstr(argv[1]);
	ft_putstr("\n");
*/	if (is_buildin(comd) == 1)
		ft_pwd();		
	else
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


int launcher(char *cmd, char *comd, t_ms *g, int i)
{
	pid_t pid, wpid;
	int status;
	
	pid = fork();
	if (pid == 0)
	{
	  // Child process
		if (launch(cmd, comd, g, i) == -1)
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
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	} 
	return 1;
}
int		find_cmd_path(char *cmd, t_ms *g)
{
	DIR				*dir;
	struct dirent	*dirp;
	int				i;
	char			*comd;
	i = 0;
	comd = get_cmd_in_line(cmd);

	while (g->path[i])
	{
		dir = opendir(g->path[i]);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, comd))
				{
					launcher(cmd, comd, g, i);
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

