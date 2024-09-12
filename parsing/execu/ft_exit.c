/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:34:55 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/11 16:06:37 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

long	ft_atoi(char *str, t_var *var)
{
	int		i;
	int		n;
	long	result;

	i = 0;
	n = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			n = -n;
		i++;
	}
	while ('0' <= str[i] && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - 48);
		if ((result * n > LONG_MAX && result * n < LONG_MIN))
		{
			printf("exit\nexit: %s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
		i++;
	}
	return (result * n);
}

void	ft_exit(t_var *var)
{
	long	num;
	int		ac;

	ac = 0;
	while (var->list->argc[ac])
		ac++;
	if (ac == 1)
	{
		printf("exit\n");
		exit(0);
	}
	else if (ac < 3)
	{
		num = ft_atoi(var->list->argc[1], var);
		if (is_num(var->list->argc[1]))
		{
			printf("exit\n");
			exit(num);
		}
		else
		{
			printf("exit\nexit: %s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
	}
	else if (ac > 2)
		printf("exit\nbash: exit: too many arguments\n");
}
