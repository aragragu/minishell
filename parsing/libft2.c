/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:50:03 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:50:47 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*ft_lstnew_cmd(void)
{
	t_cmd	*new_node;

	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->cmd = NULL;
	new_node->argc = NULL;
	new_node->redirection = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	ft_lstadd_back(t_elem **lst, t_elem *new)
{
	t_elem	*ptr;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

void	ft_lstadd_back2(t_env **lst, t_env *new)
{
	t_env	*ptr;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

void	ft_lstadd_back_garbage(t_garbage **lst, t_garbage *new)
{
	t_garbage	*ptr;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}

void	ft_lstadd_back_redi(t_redir **lst, t_redir *new)
{
	t_redir	*ptr;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
}
