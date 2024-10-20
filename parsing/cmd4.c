/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:42:22 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:42:38 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	concatinate_the_rest(t_elem **new_list, t_elem *current,
t_garbage **grb)
{
	t_elem	*new_elem;

	if (current->fd != -1)
	{
		new_elem = ft_lstnew(current->content, current->type, grb);
		new_elem->fd = current->fd;
	}
	else
		new_elem = ft_lstnew(current->content, current->type, grb);
	ft_lstadd_back(new_list, new_elem);
}

void	concatination(t_elem **list, t_garbage **garbage)
{
	t_elem	*new_list;
	t_elem	*current;

	if (!*list)
		return ;
	new_list = NULL;
	current = *list;
	while (current)
	{
		if (current && current->type < S_PACE)
			concatinate(&current, &new_list, garbage);
		else
		{
			concatinate_the_rest(&new_list, current, garbage);
			current = current->next;
		}
	}
	if (new_list)
		*list = new_list;
}
