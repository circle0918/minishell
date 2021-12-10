#include "../includes/minishell.h"
void	get_path(t_ms *g)
{
	char *path;
	t_list *tmp;
	char *pos;

	tmp = g->env;
	path = NULL;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, "PATH=");
		if (pos && pos - (char *)tmp->content == 0)
		{
			path = tmp->content;
			break;
		}
		tmp = tmp->next;
	}
	// maybe path= null or PATH= nothing
	path = ft_substr(path, 5, (ft_strlen(path) - 5));
	g->path = ft_split(path, ':');
}
char	*get_cmd_in_line(char *line)
{
	//char *cmd;
	
	//TODO PARSER ls -a but take only ls
	return (line);
}
int		find_cmd_path(char *cmd, t_ms *g)
{
	DIR				*dir;
	struct dirent	*dirp;
	int				i;

	i = 0;

	while (g->path[i])
	{
		dir = opendir(g->path[i]);
		if (dir)
		{
			while ((dirp = readdir(dir)) != NULL)
			{
				if (ft_strequ(dirp->d_name, cmd))
				{
					if (closedir(dir) == -1)
						perror("error : closedir");	
					return (1);
				}
			}
		}
		if (closedir(dir) == -1)
			perror("error : closedir");	
		i++;
	}
	return (0);
}

