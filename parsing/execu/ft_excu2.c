/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_excu2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:45:58 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/09 13:14:27 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execution(t_var *var)
{
	int	num_cmd;

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
}
