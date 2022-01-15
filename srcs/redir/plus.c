#include "../includes/minishell.h"

char **get_file(char *str)
{
    char **out_file;
    out_file = ft_split(str, '>');
 /*   int i = 0;
    while(out_file[i])
    {
        while(out_file[i])
        {
            printf("file : %s\n", out_file[i]);
         i++;
        }
    }*/
    return (out_file);
}

char *get_pwd()
{
    char buf[1024];
	char *cwd;
	
	cwd = getcwd(buf, sizeof(buf));
    return(cwd);
}

char **get_argv_redir(t_ms *g, char *cmd)
{
	char			**tab;
	char			**argv;
	int	i;
	int	argc;

	i = 0;
	argc = 1;
	tab = creat_list_arg(g, cmd);
	//print_split(tab);
	while (tab[i+1])
	{
		if (ft_strequ(tab[i+1], ">"))
		{
			//TODO: other descriptor ...
			if (ft_strequ(tab[i], "1") || ft_strequ(tab[i], "2") || ft_strequ(tab[i], "&"))
				argc = i;
			else
				argc = i + 1;
			break;
		}
		i++;
		argc++;	
	}
	//printf("arvg redir argc : %d\n", argc);	
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	argv[argc] = NULL;
	i = 0;
	while (i < argc)
	{
		argv[i] = ft_strdup(tab[i]);
		i++;
	}
	exit_free(tab);
	return (argv);	
}

char **get_env_tab(t_list *env)
{
	t_list *l;
	char **ret;
	int i;

	ret = (char **)malloc(sizeof(char *) * ft_lstsize(env));
	i = 0;
	l = env;
	while (l)
	{
		ret[i++] = ft_strdup((char*)(l->content));
		l = l->next;
	}
	return (ret);
}
	
char *get_redir_out_file(char *direct)
{
    char  **files;
    int i;
    int fd;
    char  *out_file;

    files = get_file(direct);
    i = 0;
    while (files[i])
    {
        fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0664);
		close(fd);
	//	printf("files[%d] : %s\n", i, files[i]);
		i++;
	}
	out_file = ft_strdup(files[i-1]);
	exit_free(files);
	return (out_file);
}

