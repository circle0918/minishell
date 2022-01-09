/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2021/12/10 15:13:17 by thhusser         ###   ########.fr       */
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
# include <sys/wait.h>

typedef struct	s_data
{

}				t_data;

typedef struct 	s_ms
{
				int		retcode;
				t_data	parsing;
				char 	*line;
				t_list	*env;
				char	**path;
				t_list	*cmd;
				t_list	*cmd_tmp;
				t_list	*error;
}				t_ms;

void	get_path(t_ms *g);
char	*get_cmd_in_line_th(char *line, t_ms *g);
int		find_cmd_path(char *cmd, t_ms *g);
void	init_global_struct(t_ms *g);
void	record_list(t_list **list, char *str);
void	print_list(t_list *error);
void ft_echo(char *cmd, t_ms *g);
void ft_export(char *cmd, t_ms *g);
void ft_cd(char *comd, char *cmd, t_ms *g);
void ft_pwd();
void ft_unset(char *comd, char *cmd, t_ms *g);
void	exit_free(char **str);
t_list	*ft_lst_pop_last(t_list **lst);
#endif
