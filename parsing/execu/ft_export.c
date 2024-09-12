/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:32:21 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/11 21:18:12 by ykasmi           ###   ########.fr       */
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
			printf("declare -x %s=\"%s\"\n", first->key, first->value);
		else
			printf("declare -x %s\n", first->key);
		first->flag = 1;
		j++;
	}
	init_flag(*env);
	return ;
}

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

void	ft_export(t_var *var, int i, int error)
{
	int		j;
	int		k;
	int		flag_plus;
	char	*new_val;
	char	*key;
	t_env	*index;

	flag_plus = 0;
	if (!var->list->argc[1])
		sort_env(&var->env);
	else
	{
		while (var->list->argc[++i])
		{
			if (var->list->argc[i][0] && (var->list->argc[i][0] == '_' || ft_isalpha(var->list->argc[i][0])))
			{
				j = ft_strlen(var->list->argc[i]);
				if (ft_strchr(var->list->argc[i], '='))
				{
					j = -1;
					while (var->list->argc[i][++j])
					{
						if (var->list->argc[i][j] == '=')
							break ;
					}
					if (var->list->argc[i][j - 1] == '+')
					{
						flag_plus = 1;
						j--;
					}
				}
				k = 0;
				while (++k < j)
				{
					if (!(var->list->argc[i][k] == '_' || ft_isalpha(var->list->argc[i][k]) || ft_digits(var->list->argc[i][k])))
					{
						error = 1;
						break ;
					}
				}
			}
			else
				error = 1;
			if (!error)
			{
				if (ft_strchr(var->list->argc[i], '='))
				{
					if (flag_plus)
					{
						new_val = ft_cat(var->list->argc[i], j + 2, 1);
						key = ft_cat(var->list->argc[i], j, 0);
						index = index_key(var->env, key);
						if (index && var->list->argc[i][j] == '+' && var->list->argc[i][j + 1] == '=')
							index->value = ft_strjoinnn(index->value, new_val);
						else
							ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), ft_cat(var->list->argc[i], j + 2, 1)));
					}
					else
					{
						new_val = ft_cat(var->list->argc[i], j + 1, 1);
						key = ft_cat(var->list->argc[i], j, 0);
						index = index_key(var->env, key);
						if (index)
						{
							free(index->value);
							index->value = new_val;
						}
						else
							ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), ft_cat(var->list->argc[i], j + 1, 1)));
					}
				}
				else
				{
					key = ft_cat(var->list->argc[i], j, 0);
					index = index_key(var->env, key);
					if (!index)
						ft_lstadd_backkk(&var->env, ft_lstnewww(ft_cat(var->list->argc[i], j, 0), NULL));
				}
			}
			else
				env_key_error(var->list->argc, &var->env, i, "export");
			flag_plus = 0;
			error = 0;
		}
	}
}
