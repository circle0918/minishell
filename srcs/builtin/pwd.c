#include "../includes/minishell.h"

void ft_pwd(t_ms *g)
{
	g->ret_errno = 0;
	char buf[1024];
	char *cwd;
	
	cwd = getcwd(buf, sizeof(buf));
	if(cwd == NULL)
	{
		g->ret_errno = 0;
		perror("get working directory failed.\n");
		exit(-1);
	}
	else
		printf("%s\n", cwd);
}


