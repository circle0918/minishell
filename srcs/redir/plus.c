#include "../includes/minishell.h"




char **get_file(char *str)
{
    char **out_file;
    out_file = ft_split(str, '>');
 /*   int i = 0;
    while(out_file[i])
    {
        while(out_file[i])
        {
            printf("file : %s\n", out_file[i]);
         i++;
        }
    }*/
    return (out_file);
}

/*int go_redir(char *comd, char *direct)
{
    char *out_file;
    char  **files;
    pid_t pid;
    int fd;
    int i;

    pid = fork();
    files = get_file(direct); // to free
    while (files[i])
    {
        out_file = ft_strdup(files[i]); // a free
        i++;
    }
    if(pid == -1)
    {
        perror("child process error");
        return (0);
    }
    else if(pid == 0) // child
    {
        fd = open(out_file, O_WRONLY|O_APPEND|O_CREAT|O_APPEND, 7777) //7777 = ALL access
        if(fd < 0)
        {
            perror("open file error");
            return (0);
        }
        dup2(fd, STDOUT_FILENO);
        exevp
    }
    else
    {

    }
    
}
*/