#include "../includes/minishell.h"

void ft_unset(t_ms *g)
{
//	char *str;

//	str = ft_substr(cmd, 6, (ft_strlen(cmd)-6));
	t_list *tmp;
	char *pos;
	t_list *pre;
	int		i;
	print_2Dtab(g->cmd_tab,"hello :");
	i  = 0;
	tmp = g->env;
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
				//	free(str);
					break;
				}
			}
			pre = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
