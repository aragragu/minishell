/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:48:51 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/21 22:26:27 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_function(t_var *var)
{
	fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
	g_es(127, 0);
	exit(127);
}

void	error_fork(pid_t pid)
{
	if (pid < 0)
	{
		perror("fork failed");
		return ;
	}
}

void	waitpid_func(t_var *var, struct termios *term)
{
	int	exit_stat;
	int	i;

	i = 0;
	while (waitpid(var->pid[i++], &exit_stat, 0) > 0)
		;
	//added:
	if (WIFSIGNALED(exit_stat) && WTERMSIG(exit_stat) == SIGQUIT)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, term);
		write(1, "Quit: 3\n", 8);
	}
	if (WIFSIGNALED(exit_stat) && WTERMSIG(exit_stat) == SIGINT)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, term);
		write(1, "\n", 1);
	}
	//
	update_exit_status(exit_stat);
}
