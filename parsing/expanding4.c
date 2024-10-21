/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 19:49:41 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/21 14:13:22 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_elem	*token_quots(t_elem **list, char *in, t_garbage **garbage)
{
	int	i;

	i = 0;
	while (in && in[i])
	{
		if (in[i] == '$' && in[i + 1] == '$')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("$$", garbage),
					DOUBLE_DLR, garbage));
		else if (in[i] == '$' && in[i + 1] == '?')
			ft_lstadd_back(list, ft_lstnew(ft_strdup("$?", garbage),
					EXIT_STATUS, garbage));
		else if (in[i] == '$')
			is_a_var(list, in, i, garbage);
		else if (in[i] && in[i] != '$')
			is_a_string(list, in, i, garbage);
		i += ft_strlen(ft_lstlast(*list)->content);
	}
	return (*list);
}

void	make_list(t_elem **head, char **str, t_garbage **grb)
{
	int		i;
	int		j;

	*head = NULL;
	i = ft_strlen2(str);
	j = 0;
	while (j < i)
	{
		ft_lstadd_back(head, ft_lstnew(str[j], WORD, grb));
		if (j != (i - 1))
			ft_lstadd_back(head, ft_lstnew(ft_strdup(" ", grb), S_PACE, grb));
		j++;
	}
}

void	add_to_list(t_elem **elem, t_elem *node, t_elem *head)
{
	t_elem	*current;
	t_elem	*prev;
	t_elem	*tmp;

	prev = NULL;
	tmp = NULL;
	current = *elem;
	while (current)
	{
		if (!ft_strcmp(current->content, node->content))
		{
			tmp = ft_lstlast(head);
			if (current->next)
				tmp->next = current->next;
			if (prev)
				prev->next = head;
			else
				*elem = head;
		}
		else
			prev = current;
		current = current->next;
	}
}

void	ft_split_var(t_elem **elem, t_elem *node, t_garbage **garbage)
{
	t_elem	*new_list;
	t_elem	*prev;
	t_elem	*current;
	char	**str;

	str = NULL;
	if (ft_strchr(node->content, '\t'))
		str = ft_split(node->content, '\t', garbage);
	else if (ft_strchr(node->content, ' '))
		str = ft_split(node->content, ' ', garbage);
	new_list = NULL;
	prev = NULL;
	current = *elem;
	make_list(&new_list, str, garbage);
	add_to_list(elem, node, new_list);
}

void	expand_var_quots(t_elem **elem, t_elem *node, t_env **env,
t_garbage **grb)
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
			expand_var_quots2(node, gtr, grb, list);
	}
}
