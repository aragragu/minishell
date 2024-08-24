# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/09 18:24:50 by ykasmi            #+#    #+#              #
#    Updated: 2024/08/24 17:12:07 by ykasmi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc -fsanitize=address -g
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline -lncurses
NAME = minishell
INC = minishell.h

SRCS = minishell.c ft_utils1.c ft_utils2.c ft_utils3.c ft_cd.c ft_env.c \
       ft_export.c ft_echo.c ft_exit.c ft_pwd.c ft_unset.c 

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
