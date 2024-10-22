# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 11:48:28 by aragragu          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2024/10/21 22:53:56 by ykasmi           ###   ########.fr        #
=======
#    Updated: 2024/10/22 11:43:32 by aragragu         ###   ########.fr        #
>>>>>>> origin/master
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
<<<<<<< HEAD
CFLAGS = -Wall -Wextra -Werror -I/Users/${USER}/.brew/opt/readline/include -g3 -fsanitize=address
=======
CFLAGS = -Wall -Wextra -Werror -I/Users/${USER}/.brew/opt/readline/include #3 -fsanitize=address
>>>>>>> origin/master
LDFLAGS = -L/Users/${USER}/.brew/opt/readline/lib
RM = rm -rf
SRC =	parsing/token.c parsing/token2.c parsing/token3.c parsing/token4.c parsing/main.c parsing/split.c parsing/libft.c \
		parsing/libft2.c parsing/libft3.c parsing/libft4.c parsing/libft5.c parsing/libft6.c parsing/signals.c parsing/allo.c \
		parsing/syntax_error.c parsing/syntax_error2.c parsing/syntax_error3.c parsing/expanding.c parsing/expanding2.c  \
		parsing/expanding3.c parsing/expanding4.c parsing/redirections.c parsing/redirections2.c parsing/redirections3.c \
		parsing/redirections4.c parsing/ft_itoa.c parsing/cmd.c parsing/cmd2.c parsing/cmd3.c parsing/cmd4.c  \
		execu/ft_cd.c execu/ft_exit.c execu/ft_echo.c execu/ft_pwd.c execu/ft_export.c execu/ft_env.c\
		execu/ft_unset.c execu/ft_utils1.c execu/ft_utils2.c execu/ft_utils3.c execu/ft_excu.c\
		execu/ft_utils5.c execu/ft_utils6.c ft_printf/ft_printf.c ft_printf/ft_utils.c \
		execu/ft_excu2.c execu/ft_utils7.c execu/ft_redi.c execu/ft_utils4.c execu/ft_utils8.c \
		execu/ft_export_norm.c execu/ft_export_norm2.c parsing/print_tools.c 

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