/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:33:33 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/23 13:23:47 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	norm_unset(t_env *prev, t_env *curr, t_var *var)
{
	if (prev)
		prev->next = curr->next;
	else
		var->env = var->env->next;
}

void	norm_unset_2(t_env **prev, t_env **curr, t_var **var)
{
	(*prev) = NULL;
	(*curr) = (*var)->env;
}

void	norm_unset_3(t_env *curr, t_var *var, t_env *prev, int i)
{
	while (curr)
	{
		if (ft_strcmp(curr->key, var->list->argc[i]) == 0)
		{
			norm_unset(prev, curr, var);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalpha(str[i]) && !ft_digits(str[i]) && str[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void	ft_unset(t_var *var, int i)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = var->env;
	while (var->list->argc[++i])
	{
		if (!is_valid_identifier(var->list->argc[i]))
		{
			if (!ft_strcmp(var->list->argc[i], "PATH"))
			{
				free(var->path);
				var->path = NULL;
			}
			norm_unset_3(curr, var, prev, i);
			norm_unset_2(&prev, &curr, &var);
		}
		else
		{
			ft_fprintf(2, "unset: `%s': not a valid identifier\n",
				var->list->argc[i]);
			g_es(1, 0);
		}
	}
}
