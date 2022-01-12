/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 15:31:37 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/12 14:40:35 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_split(char **split)
{
    int i = -1;
    
    while (split[++i])
        printf("%s\n", split[i]);
}

static char		**reccord_cmd_pipe(char **pipe_command, t_ms *g)
{
	int c;
	int first;
    int i;
    int j;

    i = 0;
    j = 0;
	first = 0;
	while (g->line[i++])
	{
		if (g->line[i] == '\'' || g->line[i] == '"')
			i = parseur_quotes(g, i + 1, g->line[i]);
		if (g->line[i] == '|')
		{
			c = move_space_before(g->line, i);
			if (!(pipe_command[j] = (char *)malloc(sizeof(char) * (c - first) + 1)))
				return (NULL);
			ft_strlcpy(pipe_command[j++], &g->line[first], c - first + 1);
			i = move_space_after(g->line, i);
			first = i + 1;
		}
		if (g->line[i] == '\\')
			i++; 
	}
	pipe_command[j] = (char *)malloc(sizeof(char) * (i - first) + 1);
    if (!pipe_command[j])
	    return (NULL);
	ft_strlcpy(pipe_command[j], &g->line[first], i - first + 1);
	pipe_command[j + 1] = NULL;
	return (pipe_command);
}

void    pipe_command(t_ms *g, int pipe)
{
    char **pipe_command;

    pipe_command = (char **)malloc(sizeof(char *) * (pipe + 2));
    if (!pipe_command)
        return ;
    if ((pipe_command = reccord_cmd_pipe(pipe_command, g)) == NULL)
	{
		write(2, "error: malloc failed\n", 21);
        free_split(pipe_command);
		return ;
	}
    print_split(pipe_command);
	// my_pipe(command, env);
    free_split(pipe_command);
}
