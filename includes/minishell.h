/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/20 16:07:36 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>
# include <sys/wait.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# define PATH_MAX_ENV 4096
# define DEBUG 0

typedef struct s_ms
{
	int		ret;
	char	*ret_dir;
	pid_t	pid[2];
	int		last_cmd;
	int		nb_cmd_pipe;
	char	*line;
	t_list	*env;
	char	**path;
	char	**cmd_tab;
	int		cmd_ac;
	t_list	*cmd;
	t_list	*cmd_tmp;
	t_list	*error;
	int		ret_errno;
	int		exit;
	int		unset_path;
	int		redir_out_fd;
	int		redir_in_fd;
	char	*tmp;
}			t_ms;

t_ms		*g_ms;

char	*norm_clean_line_cmd(char **dest, int count, char *str, char *tmp);
int		handle_cmd_noneed_fork(t_ms *g, char *cmd);
char	*find_cmd_in_path_tab(t_ms *g);
char	*norm_clean_line_cmd_2(t_ms *g, int count, char *str, char *tmp);
char	*norm_tmp_cote(t_ms *g, char *tmp, char *str);
char	*test_redir_flag_2(char *cmd);

void	ft_exit_plus(char **cmd);
int		checkvar(char *str, int i);

//echo $
char	*check_var_cmd(t_ms *g, char *cmd);
// int		takepath(char *str, int idx, char *tmp, t_ms *g);

//doucle char
char	**creat_list_arg(char *line);
void	print_split(char **cmd);//tmp
char	**cmd_creatnull(char **cmd, int j, int k);
int		ft_passpace(char *line, int idx);
void	print_split(char **cmd);
int		count_word(char *line);

//pipe
void	pipe_command(t_ms *g, int pipe);
void	init_pipe(t_ms *g);
void	my_pipe(char **cmd, t_ms *g);
void	preexecution(char **cmd, int fd_in[2], int fd_out[2], t_ms *g);

//utils pipe
int		move_space_after(char *str, int i);
int		move_space_before(char *str, int c);

//list
void	record_list(t_list **list, char *str);
void	print_list(t_list *error);

//free and exit
void	ft_exit(int nb, t_ms *g, int ret, char *line);
void	free_split(char **split);

//utils parsing check
int		parseur_quotes(t_ms *g, int i, int c);
int		parseur_quotes_str(char *str, int i, int c);
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
void	record_list(t_list **list, char *str);
void	print_list(t_list *error);
void	ft_echo(t_ms *g);
void	ft_export(t_ms *g);
void	ft_cd(t_ms *g);
void	ft_pwd(t_ms *g);
void	ft_unset(t_ms *g);
void	exit_free(char **str);
t_list	*ft_lst_pop_last(t_list **lst);
char	*get_env(char *str, t_list *env);
int		get_cmd_size(char *cmd);
int		check_uset_error(char *str);
char	*get_pwd(void);
int		launcher(char *cmd, char *comd, t_ms *g, char *path_i);
char	*get_abs_path(int pos, char *comd);
int		get_last_char_pos(char *s, char c);
int		exec_cmd_has_dir(char *cmd, char *comd, t_ms *g);
int		count_tab(char **tab);
void	init_redir(t_ms *g, char *cmd);
int		is_buildin_2(char *comd, t_ms *g);
int		is_buildin(char *comd, t_ms *g);
char	**init_argv(char *cmd);
char	**get_argv(t_ms *g, char *cmd);
int		do_redir(t_ms *g, char *cmd, int *out, int *in);
void	clean_redir(int *out, int *in);
int		launch_exec(char *cmd, char *comd, t_ms *g, char *path_i);
int		launch(char *cmd, char *comd, t_ms *g, char *path_i);
int		count_space(char *tmp);
void	strcpy_del_c(char *tmp, t_ms *g);
void	test_redir_flag(char *cmd, t_ms *g);
char	*get_cmd_in_line(char *line);
void	get_path(t_ms *g);
char	*init_abs_comd(char *comd, char *path_i);
char	*find_cmd_in_path_i(char *cmd, char *path_i);
void	ft_print_export(t_list *export_lst);
void	export_no_arg_0(t_list **expl, t_list **i,
			t_list **node, t_list **last);
void	export_no_arg(t_ms *g);
int		export_checker(char **tab, int i, t_ms *g);
int		export_replaced(char *ptr, char **tab, int i, t_ms *g);
t_list	*ft_lst_pop_last(t_list **lst);
void	export_append(char **tab, int i, t_ms *g);
int		ft_export_path_flag(t_ms *g);
void	ft_export(t_ms *g);
void	error_out2(char *comd, char *opt, char *msg);
char	*get_env(char *str, t_list *env);
int		set_env_replace(char *key, t_list *env, char (*s)[]);
void	set_env(char *key, char *val, t_list *env);
void	change_path(t_ms *g, int change_back);

//redir fonctions
int		ft_output(char *comd, char *direct, t_ms *g);
char	**get_file(char *str);
int		get_redir_out_file(char *cmd);
char	**get_argv_redir(char *cmd);
void	print_2Dtab(char **tab, char *str);
int		get_redir_in_file(char *cmd);
void	error_out2(char *comd, char *opt, char *msg);
char	**get_env_tab(t_list *env);
char	*get_pwd(void);
int		get_argc_redir(char **tab);
char	**get_argv_redir(char *cmd);
char	**get_env_tab(t_list *env);
int		handle_redir_in(int *fd, char **tab, int i);

//main folder
//->utils
void	begin(char **env, t_ms *g);
int		count_split(char **split);
void	norm_clean_cmd(t_ms *g, char *command);
char	*extract_string(char *str, int len);
int		contains_only_whitespaces(char *line);

//->signals
void	end(int sig);
void	recovery(int sig);

//->parseur main / ->utils_main
int		parseur(t_ms *g, int i, int res);
void	lauch_all(t_ms *g);
void	print_error_main(t_ms *g);

//->check redir
char	*ft_spaceredir(char *str, char *tmp, int idx, int i);
char	*ft_checkbackredir(t_ms *g, int i, int nb);
char	*check_in_out(t_ms *g, char *str);

//->clean
char	*norm_clean_line_cmd(char **dest, int count, char *str, char *tmp);
void	clean_line_cmd(t_ms *g);
int		check_nb_pipe(const char *str, t_ms *g);
int		clean_command(t_ms *g);

#endif
