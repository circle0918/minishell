/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/13 19:00:00 by thhusser         ###   ########.fr       */
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

// typedef struct	s_data
// {
				// une structure par commande
				// forcement que une commande ! car pas de ';'
// }				t_data;

typedef struct 	s_ms
{
				// t_data	parsing;
				char 	*line;
				t_list	*env;
				char	**path;
				t_list	*cmd;
				t_list	*cmd_tmp;
				t_list	*error;
}				t_ms;

//list
void	record_list(t_list **list, char *str);
void	print_list(t_list *error);

//free and exit
void	ft_exit(int nb, t_ms *g);
void	free_split(char **split);

void	get_path(t_ms *g);
char	*get_cmd_in_line_th(char *line, t_ms *g);
int		find_cmd_path(char *cmd, t_ms *g);
void	init_global_struct(t_ms *g);

#endif
