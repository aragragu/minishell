/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:32:21 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/09 20:21:17 by ykasmi           ###   ########.fr       */
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

	i = ft_lstsizeee(*env);
	j = 0;
	while (i && i >= j)
	{
		first = *env;
		tmp = *env;
		while (tmp)
		{
			if (tmp->flag == 0)
				first = tmp;
			tmp = tmp->next;
		}
		tmp = *env;
		while (tmp)
		{
			if (ft_strcmplist(first->key, tmp->key) == 2 && !tmp->flag)
				first = tmp;
			else if (!ft_strcmplist(first->key, tmp->key) && \
					ft_strlen(tmp->key) < ft_strlen(first->key) && !tmp->flag)
				first = tmp;
			tmp = tmp->next;
		}
		if (first->value)
			ft_fprintf(1, "declare -x %s=\"%s\"\n", first->key, first->value);
		else
			ft_fprintf(1, "declare -x %s\n", first->key);
		first->flag = 1;
		j++;
	}
	init_flag(*env);
	return ;
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
