#include "../includes/minishell.h"

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

void del()
{
	//here do not free the content in each env.
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
	while(tmp_env->next)
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

	ft_lstclear(&export_lst, &del);
}

int export_checker(char **tab, int i, t_ms *g)
{
	int j;

	//if AAA =aaa -> error : export: « =aaa » : identifiant non valable and echo $? == 1
	if (!(ft_isalpha(tab[i][0]) || tab[i][0] == '_'))//first char can't be 0-9, but can be '_'
	{
		g->ret_errno = 1;
		printf("minishell: export: '%s': not a valid identifier\n", tab[i]);
		return (1);
	}
	j = 1;
	while(tab[i][j])//if key contains char other than "a-z, A-Z, 0-9, _", it is not valid
	{
		if (tab[i][j] == '=')
			break ;
		if (!(ft_isalnum(tab[i][j]) || tab[i][j] == '_'))
		{
			g->ret_errno = 1;
			printf("minishell: export: '%s': not a valid identifier\n", tab[i]);
			return 1;
		}
		j++;
	}
	return (0);
}

int export_replaced(char* ptr, char **tab, int i, t_ms *g)
{
	t_list *tmp;
	char *pos;
	char *str;

	tmp = g->env;
	pos = NULL;
	str = ft_substr(tab[i], 0, (ptr-tab[i]));
	while (tmp)
	{
		pos = ft_strstr(tmp->content, str);
		if (pos && pos - (char *)tmp->content == 0)// if env already exist, TODO replace the old one only, don't need to add it
		{
			free(tmp->content);
			tmp->content = ft_strdup(tab[i]);
			free(str);
			return (1);
		}
		tmp = tmp->next;
	}
	free(str);
	return (0);
}

t_list	*ft_lst_pop_last(t_list **lst)
{
	t_list	*pop;
	t_list	*top;

	if (lst == NULL || *lst == NULL)
		return (NULL);
	top = *lst;
	if (top->next == NULL)
	{
		pop = top;
		top = NULL;
		return (pop);
	}
	while (top->next->next)
		top = top->next;
	pop = top->next;
	top->next = NULL;
	return (pop);
}

void export_append(char **tab, int i, t_ms *g)
{
	t_list *last;

	//record the new before the last(_=./minishell)
	last = ft_lst_pop_last(&g->env);
	record_list(&g->env, tab[i]);
	ft_lstadd_back(&g->env, last);
	//AAA= aaa ==> considered as two args. "" will be added to AAA
}

void ft_export(char *cmd, t_ms *g)
{
	int i;
	(void)cmd;
	char *strsub;

	g->ret_errno = 0;
	if (g->cmd_ac == 1) //if only export == export p : declare -x all env=
	{
		export_no_arg(g);
		return ;
	}
	strsub = ft_substr(g->cmd_tab[1], 0, 4);
	if (ft_strequ("PATH", strsub))
	{
		g->unset_path = 0;
	}
	free(strsub);
	i = 0;
	while(g->cmd_tab[++i])
	{
		if (export_checker(g->cmd_tab, i, g))
			continue ;
		char *ptr = ft_strchr(g->cmd_tab[i], '=');
		if (ptr == NULL)
			continue ;//if export AAA -> do nothing
		else
		{
			if (export_replaced(ptr, g->cmd_tab, i, g))
				continue;
			export_append(g->cmd_tab, i, g);
		}
	}
}
