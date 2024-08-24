/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 16:55:00 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:20:55 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(t_var *var)
{
	t_env	*prev;
	t_env	*curr;
	int		i;

	prev = NULL;
	curr = var->env;
	i = 0;
	while (var->list->arg[++i])
	{
		while (curr)
		{
			if (ft_strcmp(curr->key, var->list->arg[i]) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					var->env = var->env->next;
				free(curr->key);
				free(curr->val);
				free(curr);
				return ;
			}
			prev = curr;
			curr = curr->next;
		}
		prev = NULL;
		curr = var->env;
	}
}
