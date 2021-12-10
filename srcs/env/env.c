#include "../includes/minishell.h"

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

