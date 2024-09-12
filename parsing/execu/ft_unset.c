/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:33:33 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/11 21:23:48 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_var *var)
{
	t_env	*prev;
	t_env	*curr;
	int		i;

	prev = NULL;
	curr = var->env;
	i = 0;
	while (var->list->argc[++i])
	{
		while (curr)
		{
			if (ft_strcmp(curr->key, var->list->argc[i]) == 0)
			{
				if (prev)
					prev->next = curr->next;
				else
					var->env = var->env->next;
				return (free(curr->key), free(curr->value), free(curr));
			}
			prev = curr;
			curr = curr->next;
		}
		prev = NULL;
		curr = var->env;
	}
}
