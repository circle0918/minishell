# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thhusser <thhusser@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/08 16:26:34 by thhusser          #+#    #+#              #
#    Updated: 2021/12/10 12:56:40 by thhusser         ###   ########.fr        #
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
				env/env.c \
				init.c \
				
				

CC			=	clang

FLAGS		= 	-Werror -Wextra -Wall -g

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
			@$(CC) $(FLAGS) $(HEADER) $(OBJS) -o $(NAME) -L ${PATH_LIBFT} -lft 
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

