#include "../includes/minishell.h"
int		check_unset_error(char *str)
{
	int i;

	i = 1;
	if (!(ft_isalpha(str[0]) || str[0] == '_'))//first char can't be 0-9, but can be '_'
		{
			printf("minishell: uset: '%s': not a valid identifier\n", str);
			return (1);
		}
	while(str[i])
	{
//		printf("out str[i]%c\n",str[i]);
		if (!(ft_isalnum(str[i] )|| str[i] == '_'))
		{
			
//		printf("int str[i]%c\n",str[i]);
			printf("minishell: unset: '%s': not a valid identifier\n", str);
			return (1);
		}
		i++;
	}
	return (0);
}

void ft_unset(t_ms *g)
{
	t_list *tmp;
	char *pos;
	t_list *pre;
	int		i;
	i  = 1;
	tmp = g->env;

	g->ret_errno = 0;
	if (!g->cmd_tab[1])
		return;
	
	if(check_unset_error(g->cmd_tab[1]))
	{
		g->ret_errno = 1;
		return;
	}

	while (g->cmd_tab[i])
	{
		while (tmp)
		{
			pos = ft_strstr(tmp->content, g->cmd_tab[i]);
			if (pos && pos - (char *)tmp->content == 0) //first sub string and ABCD=123 ABC=123 find ABC= 
			{
				if (((char *)tmp->content)[ft_strlen(g->cmd_tab[i])] == '=')
				{
					if (tmp->next)
						pre->next = tmp->next;
					else
					{
						pre->next = NULL;
					}
					free(tmp->content);
					free(tmp);
					break;
				}
			}
			pre = tmp;
			tmp = tmp->next;
		}
		tmp = g->env;
		i++;
	}
}
