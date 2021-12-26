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
void ft_pwd()
{
	char buf[1024];
	char *cwd;
	
	cwd = getcwd(buf, sizeof(buf));
	if(cwd == NULL)
	{
		perror("get working directory failed.\n");
		exit(-1);
	}
	else
		printf("%s\n", cwd);
}
void ft_print_export(t_list *export_lst)
{
	t_list *tmp;
	int i;
	int is_first_equal;
	tmp = export_lst;
	while(tmp)
	{
		is_first_equal = 1;
		i = 0;
		printf("declare -x ");
		while(((char *)tmp->content)[i])
		{
			printf("%c", ((char *)tmp->content)[i]);
			if (((char *)tmp->content)[i] == '=' && is_first_equal)
			{
				printf("\"");
				is_first_equal = 0;
			}
			i++;
		}
		printf("\"\n");
		tmp = tmp->next;
	}
}
void export_no_arg (t_ms *g)
{
	//int l;
	t_list *export_lst;
	t_list *tmp_env;
	t_list *tmp_exp;
	t_list *tmp_exp_pre = NULL;

	tmp_env = g->env;
	export_lst = ft_lstnew(tmp_env->content); //duplicate 1st ?
	tmp_exp = export_lst;
	//printf("lst->content : %s\n", export_lst->content);
//	print_list(g->env);
tmp_env = tmp_env->next;
	while(tmp_env)
	{
	
	//    printf("env->content : %s\n", tmp_env->content);
	//	printf("--------------------------------------------\n");
		tmp_exp = export_lst;
		tmp_exp_pre = NULL;
		t_list *node;
		t_list *last = NULL;
		while(tmp_exp)
		{
		//	printf("content : %s\n", tmp_exp->content);
			if(ft_strcmp(tmp_exp->content, tmp_env->content) > 0) //env < exp
			{
				node = ft_lstnew(tmp_env->content);
				if(tmp_exp_pre)
				{
					tmp_exp_pre->next = node;
					node->next = tmp_exp;
	//	print_list(export_lst);
	//	printf("--------------------------------------------\n");
					break;
				}
				else
				{
				/*	t_list *ttmp;
					ttmp = tmp_exp;
					node->next = ttmp;
					tmp_exp = node;*/
					ft_lstadd_front(&export_lst, node);
					break;
				}	
			}
			tmp_exp_pre = tmp_exp;
			if (tmp_exp->next == NULL)
				last = tmp_exp;
			tmp_exp = tmp_exp->next;
		}
		if (last)
		{
			node = ft_lstnew(tmp_env->content);
			last->next = node;
		}
			
		tmp_env = tmp_env->next;
	}
	//print_list(export_lst);
	ft_print_export(export_lst);

	ft_lstclear(&export_lst, &ft_del_list);
}

void ft_export(char *cmd, t_ms *g)
{
	//TODO AAA= aaa / AAA= -> env : AAA=
	//if AAA =aaa -> error : export: « =aaa » : identifiant non valable and echo $? == 1
	//if only export == export p : declare -x all env=
	//if export AAA -> nothing
	// if env already exist, TODO replace the old one only, don't need to add it
	// maybe sort

	char **tab;
	int i;

	i = 1;
	g->statut = 0;
	tab = ft_split(cmd, ' ');
	if (!tab[1])
		export_no_arg(g);
	else
	{
		while(tab[i])
		{
			if (ft_isalpha(tab[i][0]) != 1)
			{
				g->statut = 1;
				printf("minishell: export: '%s': not a valid identifier\n", tab[i]);
			}
			i++;
		}
		if (g->statut == 1)
		{
			exit_free(tab);
			return;
		}
		char *ptr = ft_strchr(tab[1], '=');
		if (ptr == NULL)
		{
			exit_free(tab);
			return;	
		}
		else
		{
			t_list *tmp;
			tmp = g->env;
			char *pos = NULL;
			char *str = ft_substr(tab[1], 0, (ptr-tab[1]));
			while (tmp)
			{
				pos = ft_strstr(tmp->content, str);
				if (pos && pos - (char *)tmp->content == 0)
				{
					free(tmp->content);
					tmp->content = ft_strdup(tab[1]);
					exit_free(tab);
					return;
				}
				tmp = tmp->next;
			}
			record_list(&g->env, tab[1]);
			exit_free(tab);
		}
	}
}
void ft_unset(char *comd, char *cmd, t_ms *g)
{
	char *str;
	(void)comd;

	str = ft_substr(cmd, 6, (ft_strlen(cmd)-6));
	t_list *tmp;
	char *pos;
	t_list *pre;

	tmp = g->env;
	while (tmp)
	{
		pos = ft_strstr(tmp->content, str);
		if (pos && pos - (char *)tmp->content == 0) //first sub string and ABCD=123 ABC=123 find ABC= 
		{
			if (((char *)tmp->content)[ft_strlen(str)] == '=')
			{
				if (tmp->next)
					pre->next = tmp->next;
				else
				{
					pre->next = NULL;
				}
				
				free(tmp->content);
				free(tmp);
				free(str);
				break;
			}
		}
		pre = tmp;
		tmp = tmp->next;
	}
}
void ft_cd(char *comd, char *cmd)
{
	//TODO $home/$pwd change env PWD and OLDPWD
	char *path;
	(void)comd;

	path = ft_substr(cmd, 3, (ft_strlen(cmd)-3));

	char cwd[1024];

    if(path[0] != '/')
    {// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        ft_strcat(cwd,"/");
        ft_strcat(cwd,path);
        chdir(path);
    }else{//true for dir w.r.t. /
        chdir(path);
    }



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
		ft_cd(comd, cmd);
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
*/	if (is_buildin(comd, cmd, g) == 0)	
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
	if(ft_strcmp(comd, "export") == 0)
	{
		if (launch(cmd, comd, g, i) == -1)
	  		perror("launch error");
		return (1);
	}
	if(ft_strcmp(comd, "unset") == 0)
	{
		if (launch(cmd, comd, g, i) == -1)
	  		perror("launch error");
		return (1);
	}
	if(ft_strcmp(comd, "cd") == 0)
	{
		if (launch(cmd, comd, g, i) == -1)
	  		perror("launch error");
		return (1);
	}
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

