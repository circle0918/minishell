#include "../includes/minishell.h"
int		check_n(char **tab)
{
	int i;
	int j;

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
void ft_echo(char *cmd, t_ms *g)
{
	//TODO
	//echo hello -n = hello -n
	//echo -n hello == echo -nnn hello ===echo -n -nnnn hello = hello without '\n'
	// ' ', "", \" = ", $ env,
	//echo $? return 0 (normal)/1 (error)
	char **tab;
	int i;

	g->retcode = 0;
	tab = ft_split(cmd, ' ');
	if(!tab[1])
	{
		printf("\n");
		exit_free(tab);
		return;
	}
	i = check_n(tab);
	int n = i > 1 ? 0 : 1;
	while(tab[i])
	{
		printf("%s", tab[i]);
		i++;
		if (tab[i])
			printf(" ");
	}
	if (n)
		printf("\n");
	exit_free(tab);
}
