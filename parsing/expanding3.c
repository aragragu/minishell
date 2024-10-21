/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:48:01 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 14:12:05 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	has_a_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z'))
			return (1);
		i++;
	}
	return (0);
}

void	expand_var3(t_elem **list, t_elem *node, t_garbage **grb)
{
	t_elem	*checker;

	checker = *list;
	if (node->content)
	{
		if (has_a_char(node->content) && ft_strchr(node->content, ' ')
			&& !ft_strcmp((*checker).content, "export"))
		{
			if (checker->next && checker->next->type == S_PACE)
			{
				if (checker->next->next && checker->next->next->type == VAR
					&& !ft_strchr(checker->next->next->content, '='))
					ft_split_var(list, node, grb);
				else
					return ;
			}
		}
		else
			ft_split_var(list, node, grb);
	}
}

void	expand_var(t_elem **elem, t_elem *node, t_env **env, t_garbage **grb)
{
	int		i;
	char	*gtr;
	t_elem	*checker;
	int		flag;
	t_env	*list;

	checker = *elem;
	gtr = node->content;
	i = 0;
	flag = 0;
	list = *env;
	if (gtr[i] == '$')
	{
		if (gtr[i + 1] >= '0' && gtr[i + 1] <= '9')
		{
			node->content = ft_strdup("", grb);
			return ;
		}
		else
			expand_var2(node, gtr, grb, list);
	}
	if (node->content && (ft_strchr(node->content, '\t')
			|| ft_strchr(node->content, ' ')))
		expand_var3(elem, node, grb);
}

void	expand_d_qouts(t_env **env, char **ptr, t_garbage **garbage)
{
	t_elem		*list;
	t_elem		*current;
	char		*str;

	list = NULL;
	current = NULL;
	list = token_quots(&list, *ptr, garbage);
	current = list;
	str = ft_strdup("", garbage);
	while (current)
	{
		if (current->type == VAR)
			expand_var_quots(&list, current, env, garbage);
		if (current->type == EXIT_STATUS)
			current->content = ft_itoa(g_es(0, 1), garbage);
		if (!current->content)
			current->content = ft_strdup("", garbage);
		str = ft_strjoin(str, current->content, garbage);
		current = current->next;
	}
	*ptr = str;
}

void	expand_d_qouts_2(t_env **env, char **ptr, t_garbage **garbage)
{
	t_elem		*list;
	t_elem		*current;
	char		*str;

	list = NULL;
	current = NULL;
	list = token_quots(&list, *ptr, garbage);
	current = list;
	str = ft_strdup("", garbage);
	while (current)
	{
		if (current->type == VAR)
			expand_herdoc(&current->content, env, garbage);
		if (!current->content)
			current->content = ft_strdup("", garbage);
		str = ft_strjoin(str, current->content, garbage);
		current = current->next;
	}
	*ptr = str;
}
