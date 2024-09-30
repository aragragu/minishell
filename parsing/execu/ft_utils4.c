/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 11:58:26 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/30 12:14:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_builtins(char *str)
{
	if (!str)
		return (-1);
	else if (!ft_strcmp(str, "echo") || !ft_strcmp(str, "cd") || \
			!ft_strcmp(str, "exit") || !ft_strcmp(str, "pwd") || \
			!ft_strcmp(str, "export") || !ft_strcmp(str, "unset") || \
			!ft_strcmp(str, "env"))
		return (1);
	return (0);
}

void	ft_builtins(t_var *var, char *str, t_cmd **cmd)
{
	(void)cmd;
	if (!ft_strcmp(str, "echo"))
		ft_echo(var);
	else if (!ft_strcmp(str, "exit"))
		ft_exit(var);
	else if (!ft_strcmp(str, "pwd"))
		ft_pwd();
	else if (!ft_strcmp(str, "cd"))
		ft_cd(var);
	else if (!ft_strcmp(str, "unset"))
		ft_unset(var, 0);
	else if (!ft_strcmp(str, "export"))
		ft_export(var, 0, 0);
	else if (!ft_strcmp(str, "env"))
		ft_env(var);
}

int	count_env(t_env *envv)
{
	int	count;

	count = 0;
	while (envv)
	{
		count++;
		envv = envv->next;
	}
	return (count);
}
