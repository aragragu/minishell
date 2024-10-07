# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 11:48:28 by aragragu          #+#    #+#              #
#    Updated: 2024/10/07 21:02:30 by aragragu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I/Users/aragragu/.brew/opt/readline/include #-g -fsanitize=address
LDFLAGS = -L/Users/aragragu/.brew/opt/readline/lib
RM = rm -rf
SRC = main.c split.c libft.c token.c syntax_error.c expanding.c redirections.c ft_itoa.c cmd.c \
	  execu/ft_cd.c execu/ft_exit.c execu/ft_echo.c execu/ft_pwd.c execu/ft_export.c execu/ft_env.c\
	  execu/ft_unset.c execu/ft_utils1.c execu/ft_utils2.c execu/ft_utils3.c execu/ft_excu.c\
	  execu/ft_utils5.c execu/ft_utils6.c ft_printf/ft_printf.c ft_printf/ft_utils.c \
	  execu/ft_excu2.c execu/ft_utils7.c execu/ft_redi.c execu/ft_utils4.c execu/ft_utils8.c 

OBJ = $(SRC:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) -lreadline -lhistory

%.o : %.c minishell.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re : fclean all