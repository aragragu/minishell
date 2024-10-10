/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_norm2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:33:55 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/10 22:30:44 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*index_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(key, tmp->key))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ex1_norm(t_var *var, int i, int *error)
{
	while (var->list->argc[++i])
	{
		ex2_norm(var, i, error);
		if (!*error)
		{
			if (ft_strchr(var->list->argc[i], '='))
				ex3_norm(var, i);
			else
			{
				var->key = ft_cat(var->list->argc[i], var->j, 0);
				var->index = index_key(var->env, var->key);
				if (!var->index)
					ft_lstadd_backkk(&var->env, ft_lstnewww(var->key, NULL));
				else
					free(var->key);
			}
		}
		else
			error_function_expo(var, i);
		var->flag_plus = 0;
		*error = 0;
	}
}

void	swap_nodes(t_env **first, t_env **tmp)
{
	if (ft_strcmplist((*first)->key, (*tmp)->key) == 2 && !(*tmp)->flag)
		(*first) = (*tmp);
	else if (!ft_strcmplist((*first)->key, (*tmp)->key) && \
			ft_strlen((*tmp)->key) < ft_strlen((*first)->key) && !(*tmp)->flag)
		(*first) = (*tmp);
}

void	print_export(t_env *first)
{
	if (first->value)
		ft_fprintf(1, "declare -x %s=\"%s\"\n", first->key, first->value);
	else
		ft_fprintf(1, "declare -x %s\n", first->key);
}
