# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 11:48:28 by aragragu          #+#    #+#              #
#    Updated: 2024/08/17 13:21:33 by aragragu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
RM = rm -rf
SRC = main.c split.c libft.c token.c syntax_error.c expanding.c redirections.c ft_itoa.c cmd.c
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
 