#include "../includes/minishell.h"
char	*get_cmd_in_line(char *line)
{
	char *cmd;
	int i;
	int pos;
	//TODO PARSER ls -a but only take ls
	i = 0;
	pos = 0;
	while (ft_isspace(line[i]) == 1)
	{
		pos = i;
		i++;
	}
	if (i != 0)
		pos++;
	//printf("pos : %d i : %d\n", pos, i);
	while(ft_isspace(line[i]) == 0)
	{
		i++;
	}
	//printf("pos : %d i : %d\n", pos, i);
	cmd = ft_substr(line, pos, i-pos);
//	printf("%s", cmd);
	
	return (cmd); //need to free
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
	// maybe path= null or PATH= nothing
	path = ft_substr(path_tmp, 5, (ft_strlen(path_tmp) - 5));
	free(path_tmp);
	g->path = ft_split(path, ':');
	free(path);
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
					closedir(dir);
					free(comd);
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

