/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 16:24:56 by thhusser          #+#    #+#             */
/*   Updated: 2022/01/19 19:04:36 by thhusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_pwd(t_ms *g)
{
	char	buf[1024];
	char	*cwd;

	g->ret_errno = 0;
	cwd = getcwd(buf, sizeof(buf));
	if (cwd == NULL)
	{
		g->ret_errno = 0;
		perror("get working directory failed.\n");
		exit(-1);
	}
	else
		printf("%s\n", cwd);
}
