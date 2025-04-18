/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_norm.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 17:31:43 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/28 13:54:04 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_function_expo(t_var *var, int i)
{
	ft_fprintf(2, "export: `%s': not a valid identifier\n", var->list->argc[i]);
	var->if_valid = false;
	g_es(1, 0);
}

void	ex2_norm(t_var *var, int i, int *error)
{
	if (var->list->argc[i][0] && (var->list->argc[i][0] == '_' \
		|| ft_isalpha(var->list->argc[i][0])))
	{
		ex5_norm(var, i);
		var->k = 0;
		while (++var->k < var->j)
		{
			if (!(var->list->argc[i][var->k] == '_' \
				|| ft_isalpha(var->list->argc[i][var->k]) \
				|| ft_digits(var->list->argc[i][var->k])))
			{
				*error = 1;
				break ;
			}
		}
	}
	else
		*error = 1;
}

void	ex3_norm(t_var *var, int i)
{
	if (var->flag_plus)
		ex4_norm(var, i);
	else
	{
		var->new_val = ft_cat(var->list->argc[i], var->j + 1, 1);
		ft_lstadd_back_garbage(&var->garb, ft_lstnew_garbage(var->new_val));
		var->key = ft_cat(var->list->argc[i], var->j, 0);
		var->index = index_key(var->env, var->key);
		if (var->index)
		{
			var->index->value = var->new_val;
			free(var->key);
		}
		else
			ft_lstadd_backkk(&var->env, ft_lstnew2(var->key, var->new_val,
					&var->garb));
	}
}

void	ex4_norm(t_var *var, int i)
{
	char	*temp;
	char	*ptr;

	ptr = NULL;
	var->new_val = ft_cat(var->list->argc[i], var->j + 2, 1);
	var->key = ft_cat(var->list->argc[i], var->j, 0);
	var->index = index_key(var->env, var->key);
	if (var->index && var->list->argc[i][var->j] == '+' \
		&& var->list->argc[i][var->j + 1] == '=')
	{
		if (!var->index->value)
			var->index->value = var->new_val;
		else
		{
			temp = ft_strjoin(var->index->value, var->new_val, &var->garb);
			ft_lstadd_backkk(&var->env, ft_lstnew2(NULL, var->index->value,
					&var->garb));
			var->index->value = temp;
			free(var->new_val);
		}
		free(var->key);
	}
	else
		ft_lstadd_backkk(&var->env, ft_lstnew2(var->key, var->new_val,
				&var->garb));
}

void	ex5_norm(t_var *var, int i)
{
	var->j = ft_strlen(var->list->argc[i]);
	if (ft_strchr(var->list->argc[i], '='))
	{
		var->j = -1;
		while (var->list->argc[i][++var->j])
		{
			if (var->list->argc[i][var->j] == '=')
				break ;
		}
		if (var->list->argc[i][var->j - 1] == '+')
		{
			var->flag_plus = 1;
			var->j--;
		}
	}
}
