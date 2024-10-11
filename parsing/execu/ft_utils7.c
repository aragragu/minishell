/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:48:51 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/11 15:55:20 by aragragu         ###   ########.fr       */
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

void	waitpid_func(t_var *var)
{
	int	exit_stat;
	(void)var;

	while (waitpid(-1, &exit_stat, 0) > 0)
		;
	update_exit_status(exit_stat);
}
