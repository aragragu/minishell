/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:48:51 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/09 13:13:45 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_function(t_var *var)
{
	fprintf(stderr, "minishell: %s: command not found\n", var->list->argc[0]);
	var->exit_num = 127;
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
	while (waitpid(-1, &var->exit_num, 0) > 0)
		;
	update_exit_status(var, var->exit_num);
}
