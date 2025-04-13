/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 17:02:07 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/29 17:19:54 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_variables(t_var *var)
{
	free_garbage(&var->garbage);
	var->linked_list = NULL;
	var->garbage = NULL;
	var->list = NULL;
}

void	initialize_signals(void)
{
	g_sig = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

void	read_input_loop(t_var *var, struct termios *original_termios)
{
	char	*input;
	int		i;

	while (1)
	{
		initialize_signals();
		input = readline("➜ (minishell)✗ ");
		i = fill_linked_list(input, var);
		free(input);
		if (i == 1)
			break ;
		else if (i == 2)
			continue ;
		if (g_sig == 0 && var->list)
		{
			execution(var);
			tcsetattr(STDIN_FILENO, TCSANOW, original_termios);
		}
		reset_variables(var);
	}
}

void	read_input(char **env)
{
	struct termios	original_termios;
	t_var			var;

	initialize_variables(&var, env);
	tcgetattr(STDIN_FILENO, &original_termios);
	read_input_loop(&var, &original_termios);
	exit(g_es(0, 1));
}

int	main(int argc, char *argv[], char **env)
{
	(void)argv;
	if (argc == 1)
		read_input(env);
	else
		printf("no argument needed\n");
	return (0);
}
