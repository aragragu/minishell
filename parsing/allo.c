/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:28:00 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 18:56:44 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_nodes(t_elem **tmp, int *flag, char **buff, t_var *data)
{
	while (*tmp && (*tmp)->type < S_PACE)
	{
		if ((*tmp)->type == S_QOUTS || (*tmp)->type == D_QOUTS)
			*flag = 1;
		(*tmp)->ignore[0] = 1;
		*buff = ft_strjoin(*buff, (*tmp)->content, &data->garbage);
		*tmp = (*tmp)->next;
	}
}

void	handle_heredoc(t_elem *list, t_var *data)
{
	t_elem	*tmp;
	char	*buff;
	int		flag;

	flag = 0;
	buff = ft_strdup("", &data->garbage);
	tmp = list->next;
	if (tmp && tmp->type == S_PACE)
	{
		tmp->ignore[0] = 1;
		tmp = tmp->next;
	}
	process_nodes(&tmp, &flag, &buff, data);
	list->content = buff;
	if (flag)
		list->ignore[1] = 1;
	open_herdoc(&list, &data->env, &data->garbage);
}

void	expand_d_qouts4(t_elem *node, char *gtr, t_garbage **grb, t_env *env)
{
	int		flag;

	flag = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, gtr + 1))
		{
			if (env->value)
			{
				node->content = ft_strdup(env->value, grb);
				flag = 1;
				break ;
			}
			else
				break ;
		}
		env = env->next;
	}
	if (!flag)
		node->content = NULL;
}

void	expand_var_quots2(t_elem *node, char *gtr, t_garbage **grb, t_env *env)
{
	int		flag;

	flag = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, gtr + 1))
		{
			if (env->value)
			{
				node->content = ft_strdup(env->value, grb);
				flag = 1;
				break ;
			}
			else
				break ;
		}
		env = env->next;
	}
	if (!flag)
		node->content = NULL;
}
