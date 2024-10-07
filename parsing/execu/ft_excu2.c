/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:45:58 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/07 20:16:19 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_var *var)
{
	int	num_cmd;

	var->list->fd[0] = dup(0);
	var->list->fd[1] = dup(1);
	if (var->list->cmd || contains_red(var) == 0)
	{
		num_cmd = calculate_cmd(var);
		if ((num_cmd > 1 || contains_red(var) == 0))
			execute_pipe(num_cmd, var, -1, STDIN_FILENO);
		else
		{
			if (check_builtins(var->list->cmd))
				ft_builtins(var, var->list->cmd, &var->list);
			else
				ft_exc(var);
		}
	}
	dup2(var->list->fd[0], STDIN_FILENO);
	dup2(var->list->fd[1], STDOUT_FILENO);
	close(var->list->fd[0]);
	close(var->list->fd[1]);
}
