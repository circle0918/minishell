#include "../includes/minishell.h"
#include <string.h>
int		check_n(char **tab)
{
	int i;
	int j;

	//echo hello -n ==> hello -n
	//echo -n hello == echo -nnn hello === echo -n -nnnn hello ==> hello without '\n'
	i = 1;
	while(tab[i])
	{
		if (tab[i][0] != '-')
			break;
		if (!tab[i][1] || tab[i][1] != 'n')
			break;
		j = 2;
		while(tab[i][j])
		{
			if (tab[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
/*
void echo_printf(char *s)
{
	int is_escape;
	int is_sq;//single quote
	int is_dq;//double quote
	char *p;
	char *begin;
	char *ss;

	p = s;
	while(p)
	{
		if (*p == ''')
		{
		}
		else if (*p == '"')
		{
		}
		else
		p++;
	}
}
*/
void ft_echo(char *cmd, t_ms *g)
{
	//TODO
	// ' ', "", \" ==> ", $ env,
	//echo $? return 0 (normal)/1 (error)
	char **tab;
	int i;
	int is_opt_n;

	g->retcode = 0;
	tab = ft_split(cmd, ' ');
	if(!tab[1])
	{
		printf("\n");
		exit_free(tab);
		return;
	}
	i = check_n(tab);
	is_opt_n = i > 1 ? 1 : 0;
	while(tab[i])
	{
		printf("%s", tab[i]);
		i++;
		if (tab[i])
			printf(" ");
	}
	if (!is_opt_n)
		printf("\n");
	exit_free(tab);
}
