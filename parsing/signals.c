/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:05:51 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 00:13:57 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_hand_sig_qui(int sig)
{
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		printf("Quit: 3\n");
		g_es(131, 0);
	}
}

void	signal_handler(int sig)
{
	if (sig == SIGINT && !waitpid(-1, NULL, WNOHANG))
		return ;
	if (sig == SIGINT)
	{
		// if (!waitpid(-1, NULL, WNOHANG))
		// 	return ;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_es(1, 0);
	}
	if (sig == SIGQUIT)
		return ;
}

int	g_es(int stat, int flag)
{
	static int	exit_status;

	if (flag == 0)
	{
		exit_status = stat;
	}
	return (exit_status);
}

void	s_handler(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		g_sig = 2;
		close(0);
	}
}

void	sigint_herdoc(void)
{
	int	std_in;

	std_in = open(ttyname(STDERR_FILENO), O_RDONLY, 0644);
	if (std_in < 0)
		perror("open");
}
