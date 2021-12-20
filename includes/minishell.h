/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/20 14:52:45 by thhusser         ###   ########.fr       */
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

# define DEBUG 1

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
				// t_list	*cmd;
				// t_list	*cmd_tmp;
				t_list	*error;
}				t_ms;

//list
void	record_list(t_list **list, char *str);
void	print_list(t_list *error);

//free and exit
void	ft_exit(int nb, t_ms *g);
void	free_split(char **split);

//utils parsing check
int		parseur_quotes(t_ms *g, int i, int c);
int		parsing_redirection_out(int i, int res, t_ms *g);
int		parsing_redirection_in(int i, int res, t_ms *g);
int		parsing_pipe(int i, int res, t_ms *g);
int		parse_error(int res, char *erreur, t_ms *g);

//clean redir
char	*get_tmp_che(char *tmp, char *str, int *i, int *j);
int		ft_nbchevron2(const char *str, int i);
int		ft_spacechevron(char *str, int i);
int		ft_nbchevron(const char *str);
char	*ft_checkredir2(char *str, char *tmp, int i, int j);
char	*ft_checkredir(char *str);

void	get_path(t_ms *g);
char	*get_cmd_in_line_th(char *line, t_ms *g);
int		find_cmd_path(char *cmd, t_ms *g);
void	init_global_struct(t_ms *g);

#endif
