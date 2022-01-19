#include "../includes/minishell.h"
#include <string.h>

int	check_n(char **tab)
{
	int	i;
	int	j;

	i = 1;
	while (tab[i])
	{
		if (tab[i][0] != '-')
			break ;
		if (!tab[i][1] || tab[i][1] != 'n')
			break ;
		j = 2;
		while (tab[i][j])
		{
			if (tab[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

void	ft_echo(t_ms *g)
{
	int	i;
	int	is_opt_n;

	g->ret_errno = 0;
	if (g->cmd_ac == 1)
	{
		printf("\n");
		return ;
	}
	i = check_n(g->cmd_tab);
	if (i > 1)
		is_opt_n = 1;
	else
		is_opt_n = 0;
	while (g->cmd_tab[i])
	{
		printf("%s", g->cmd_tab[i]);
		i++;
		if (g->cmd_tab[i])
			printf(" ");
	}
	if (!is_opt_n)
		printf("\n");
}
