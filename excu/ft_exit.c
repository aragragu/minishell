/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 16:52:45 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:28:30 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_num(const char *str)
{
	while (*str)
	{
		if (!ft_digits(*str) && *str != '-')
			return (0);
		str++;
	}
	return (1);
}

void	ft_exit(t_var *var)
{
	int	num;
	int	ac;

	ac = 0;
	while (var->list->arg[ac])
		ac++;
	if (ac == 2)
	{
		if (is_num(var->list->arg[1]))
		{
			num = atoi(var->list->arg[1]);
			printf("exit\n");
			exit(num);
		}
		else
		{
			printf("exit\nexit: %s: numeric arg required\n", var->list->arg[1]);
			exit(255);
		}
	}
	else if (ac > 2)
		printf("exit\nbash: exit: too many arguments\n");
}
