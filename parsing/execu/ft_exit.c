/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:34:55 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/05 18:50:42 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	norm_ft_atoi(t_var *var)
{
	printf("exit\n%s: numeric arg required\n", var->list->argc[1]);
	var->exit_num = 255;
	exit(255);
}

void	norm_ft_atoi2(char *str, int i, int sign)
{
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
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
	norm_ft_atoi2(str, i, sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		prev_result = result;
		result = result * 10 + (str[i] - '0');
		if ((result < prev_result) || (sign == -1 && - result > prev_result))
			norm_ft_atoi(var);
		if ((result > LONG_MAX) || (sign == -1 && - result < LONG_MIN))
			norm_ft_atoi(var);
		i++;
	}
	return (result * sign);
}

void	norm_ft_exit(t_var *var, long num, int flag)
{
	if (flag == 0)
	{
		ft_fprintf(0, "exit\n");
		var->exit_num = num;
		exit(num);
	}
	else if (flag == 1)
	{
		ft_fprintf(0, "exit\n");
		printf("exit: %s: numeric arg required\n", var->list->argc[1]);
		var->exit_num = 255;
		exit(255);
	}
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
			norm_ft_exit(var, num, 0);
		else
			norm_ft_exit(var, num, 1);
	}
	else if (ac > 2)
	{
		ft_fprintf(0, "exit\n");
		printf("minishell: exit: too many arguments\n");
	}
}
