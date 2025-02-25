# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/08 16:26:34 by thhusser          #+#    #+#              #
#    Updated: 2022/01/20 16:08:16 by thhusser         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

_NC=`tput sgr0`
_RED=\033[0;31m
_GREEN=\033[0;32m
_YELLOW=\033[0;33m
_BLUE=\033[0;34m
_PURPLE=\033[0;95m
_CYAN=\033[0;36m
_WHITE=\033[0;37m

NAME		= minishell

SRCS		= $(addprefix ./srcs/, ${SRCS_FILES})

SRCS_FILES	= \
				main.c \
				init.c \
				builtin/env.c \
				builtin/env_1.c \
				builtin/env_2.c \
				builtin/env_3.c \
				builtin/env_4.c \
				builtin/env_5.c \
				builtin/env_6.c \
				builtin/echo.c \
				builtin/export.c \
				builtin/export_1.c \
				builtin/cd.c \
				builtin/cd_1.c \
				builtin/pwd.c \
				builtin/unset.c \
				exit_free.c \
				list.c \
				parsing_quotes/check_parsing_out.c \
				parsing_quotes/check_parsing_in.c \
				parsing_quotes/check_parsing_pipe.c \
				parsing_quotes/check_parsing.c \
				clean_redir_in_out.c \
				pipe.c \
				utils_pipe.c \
				new_double_cmd.c \
				redir/plus.c \
				redir/plus_1.c \
				variables_1.c \
				utils_clean_redir.c \
				utils_double_cmd.c \
				utils_variable.c \
				main_folder/signal.c \
				main_folder/utils.c \
				main_folder/parseur_main.c \
				main_folder/utils_main.c \
				main_folder/check_redir_main.c \
				main_folder/clean.c \



CC			=	clang

FLAGS		= 	-Werror -Wextra -Wall -fcommon

LFLAGS		= 	-lreadline

PATH_LIBFT 	= ./libft/

HEADER		= -I ./includes

OBJS		= ${SRCS:.c=.o}

RM 			= rm -f

.c.o:
				@printf "$(_WHITE)Generating $(NAME) objects... %-33.33s\r$(_NC)" $@
				@clang ${FLAGS} ${HEADER} -c $< -o $(<:.c=.o)

all:		${NAME}

$(NAME): 	${OBJS}
			@make -C libft/
			@echo ""
			@$(CC) $(FLAGS) $(HEADER) $(OBJS) -o $(NAME) -L ${PATH_LIBFT} -lft $(LFLAGS)
			@echo "$(_GREEN)Generating $(NAME)$(_NC)"

clean:
			@make clean -C ./libft
ifneq ($(wildcard $(OBJS)),)
	@$(RM) $(OBJS)
	@echo "$(_GREEN)Deletes objects files $(NAME)$(_NC)"
endif

fclean:		clean
			@make fclean -C ./libft
ifneq ($(wildcard $(NAME)),)
	@$(RM) $(NAME)
	@echo "$(_GREEN)Delete $(NAME)$(_NC)"
endif

re:			fclean all

.PHONY: 	all fclean clean re

