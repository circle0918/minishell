#include "../includes/minishell.h"

void	ft_print_export(t_list *export_lst)
{
	t_list	*tmp;
	int	i;
	int	is_first_equal;

	tmp = export_lst;
	while (tmp)
	{
		is_first_equal = 1;
		i = 0;
		printf("declare -x ");
		while (((char *)tmp->content)[i])
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

void	export_no_arg_0(t_list **expl, t_list **i, t_list **node, t_list **last)
{
	t_list	*tmp_exp_pre;
	t_list	*tmp_exp;

	tmp_exp = *expl;
	tmp_exp_pre = NULL;
	while (tmp_exp)
	{
		if (ft_strcmp((tmp_exp)->content, (*i)->content) > 0)
		{
			*node = ft_lstnew((*i)->content);
			if (tmp_exp_pre)
			{
				tmp_exp_pre->next = *node;
				(*node)->next = tmp_exp;
			}
			else
				ft_lstadd_front(expl, *node);
			break ;
		}
		tmp_exp_pre = tmp_exp;
		if (tmp_exp->next == NULL)
			*last = tmp_exp;
		tmp_exp = tmp_exp->next;
	}
}

void	export_no_arg(t_ms *g)
{
	t_list	*export_lst;
	t_list	*tmp_env;
	t_list	*last;
	t_list	*node;

	tmp_env = g->env;
	export_lst = ft_lstnew(tmp_env->content);
	tmp_env = tmp_env->next;
	while (tmp_env->next)
	{
		last = NULL;
		export_no_arg_0(&export_lst, &tmp_env, &node, &last);
		if (last)
		{
			node = ft_lstnew(tmp_env->content);
			last->next = node;
		}
		tmp_env = tmp_env->next;
	}
	ft_print_export(export_lst);
}

int	export_checker(char **tab, int i, t_ms *g)
{
	int	j;

	if (!(ft_isalpha(tab[i][0]) || tab[i][0] == '_'))
	{
		g->ret_errno = 1;
		printf("minishell: export: '%s': not a valid identifier\n", tab[i]);
		return (1);
	}
	j = 1;
	while (tab[i][j])
	{
		if (tab[i][j] == '=')
			break ;
		if (!(ft_isalnum(tab[i][j]) || tab[i][j] == '_'))
		{
			g->ret_errno = 1;
			printf("minishell: export: '%s': not a valid identifier\n", tab[i]);
			return (1);
		}
		j++;
	}
	return (0);
}

int	export_replaced(char *ptr, char **tab, int i, t_ms *g)
{
	t_list	*tmp;
	char	*pos;
	char	*str;

	tmp = g->env;
	pos = NULL;
	str = ft_substr(tab[i], 0, (ptr - tab[i]));
	while (tmp)
	{
		pos = ft_strstr(tmp->content, str);
		if (pos && pos - (char *)tmp->content == 0)
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

void	export_append(char **tab, int i, t_ms *g)
{
	t_list	*last;

	last = ft_lst_pop_last(&g->env);
	record_list(&g->env, tab[i]);
	ft_lstadd_back(&g->env, last);
}

int	ft_export_path_flag(t_ms *g)
{
	char	*strsub;

	g->ret_errno = 0;
	if (g->cmd_ac == 1)
	{
		export_no_arg(g);
		return (1);
	}
	strsub = ft_substr(g->cmd_tab[1], 0, 4);
	if (ft_strequ("PATH", strsub))
	{
		g->unset_path = 0;
	}
	free(strsub);
	return (0);
}

void	ft_export(t_ms *g)
{
	int	i;
	char	*ptr;

	if (ft_export_path_flag(g) == 1)
		return ;
	i = 0;
	while (g->cmd_tab[++i])
	{
		if (export_checker(g->cmd_tab, i, g))
			continue ;
		ptr = ft_strchr(g->cmd_tab[i], '=');
		if (ptr == NULL)
			continue ;
		else
		{
			if (export_replaced(ptr, g->cmd_tab, i, g))
				continue ;
			export_append(g->cmd_tab, i, g);
		}
	}
}
