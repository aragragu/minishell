/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:33:11 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/01 17:31:36 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_n_option(const char *str)
{
	int	i;

	i = 1;
	while (*str)
	{
		if (str[0] == '-')
		{
			while (str[i])
			{
				if (str[i] != 'n')
					return (0);
				i++;
			}
			return (1);
		}
		str++;
	}
	return (0);
}

void	ft_echo(t_var *var)
{
	int	newline;
	int	i;

	i = 1;
	newline = 1;
	while (var->list->argc && var->list->argc[i] \
		&& check_n_option(var->list->argc[i]))
	{
		newline = 0;
		i++;
	}
	while (var->list->argc && var->list->argc[i])
	{
		ft_putstr(var->list->argc[i]);
		if (var->list->argc[i + 1])
			ft_putstr(" ");
		i++;
	}
	if (newline)
		ft_putstr("\n");
}
