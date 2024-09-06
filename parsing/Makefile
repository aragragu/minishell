# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 11:48:28 by aragragu          #+#    #+#              #
#    Updated: 2024/09/06 17:08:25 by ykasmi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -O3 -g -fsanitize=address
RM = rm -rf
SRC = main.c split.c libft.c token.c syntax_error.c expanding.c redirections.c ft_itoa.c cmd.c \
	  ft_builtins.c 
OBJ = $(SRC:%.c=%.o)
	

all : $(NAME)

$(NAME) : $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

%.o : %.c minishell.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all
 