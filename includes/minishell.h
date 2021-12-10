/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/10 12:56:29 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "signal.h"
# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>

typedef struct 	s_ms
{
				char 	*line;
				t_list	*env;
				char	**path;
				t_list	*cmd;
				t_list	*error;
}				t_ms;

void	get_path(t_ms *g);
char	*get_cmd_in_line(char *line);
int		find_cmd_path(char *cmd, t_ms *g);
void	init_global_struct(t_ms *g);

#endif
