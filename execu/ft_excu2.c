/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:45:58 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/22 12:19:37 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_var *var)
{
	int	num_cmd;

	if (!var->list->cmd && !var->list->redirection)
		return ;
	if (var->list->cmd[0] || contains_red(var) == 0)
	{
		//added:
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		//
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
}
