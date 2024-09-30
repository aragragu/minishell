/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:34:55 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/30 13:03:18 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_num(const char *str)
{
	if (*str == '-' && *(str + 1) == '\0')
		return (0);
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
	int		sign;
	long	result;
	long	prev_result;

	i = 0;
	sign = 1;
	result = 0;
	prev_result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		prev_result = result;
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result < prev_result) \
			|| (sign == -1 && - result > prev_result))
		{
			printf("exit\n%s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
		if ((sign == 1 && result > LONG_MAX) \
			|| (sign == -1 && - result < LONG_MIN))
		{
			printf("exit\n%s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
		i++;
	}
	return (result * sign);
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
		ft_fprintf(1, "exit\n");
		exit(0);
	}
	else if (ac < 3)
	{
		num = ft_atoi(var->list->argc[1], var);
		if (is_num(var->list->argc[1]))
		{
			ft_fprintf(0, "exit\n");
			exit(num);
		}
		else
		{
			ft_fprintf(0, "exit\n");
			printf("exit: %s: numeric arg required\n", var->list->argc[1]);
			exit(255);
		}
	}
	else if (ac > 2)
	{
		ft_fprintf(0, "exit\n");
		printf("minishell: exit: too many arguments\n");
	}
}
