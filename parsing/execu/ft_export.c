/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:32:21 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/10 22:28:48 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmplist(char *s1, char *s2)
{
	int	i;

	i = -1;
	while (s1[++i])
	{
		if (s1[i] < s2[i])
			return (1);
		if (s1[i] > s2[i])
			return (2);
	}
	return (0);
}

void	init_flag(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		tmp->flag = 0;
		tmp = tmp->next;
	}
}

void	sort_env(t_env **env)
{
	t_env	*tmp;
	t_env	*first;
	int		i;
	int		j;

	(1) && (i = ft_lstsizeee(*env), j = 0, 0);
	while (i && i >= j)
	{
		(1) && (first = *env, tmp = *env, 0);
		while (tmp)
		{
			(tmp->flag == 0) && (first = tmp, 0);
			tmp = tmp->next;
		}
		tmp = *env;
		while (tmp)
		{
			swap_nodes(&first, &tmp);
			tmp = tmp->next;
		}
		print_export(first);
		first->flag = 1;
		j++;
	}
	return (init_flag(*env));
}

void	ft_export(t_var *var, int i, int error)
{
	var->key = NULL;
	var->flag_plus = 0;
	var->if_valid = true;
	if (!var->list->argc[1])
		sort_env(&var->env);
	else
		ex1_norm(var, i, &error);
	if (var->if_valid == true)
		var->exit_num = 0;
}
