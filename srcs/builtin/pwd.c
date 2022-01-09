#include "../includes/minishell.h"

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


