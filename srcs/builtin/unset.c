#include "../includes/minishell.h"

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
