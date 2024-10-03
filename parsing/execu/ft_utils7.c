/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:48:51 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/03 13:35:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_function(t_var *var)
{
	(void)var;
	fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
	var->exit_num = 127;
	exit(127);
}

void	error_fork(pid_t pid)
{
	if (pid < 0)
	{
		perror("fork failed");
		exit(0);
	}
}

void	waitpid_func(void)
{
	while (waitpid(-1, NULL, 0) > 0)
		;
}
