/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:50:54 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:51:20 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

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

t_elem	*ft_lstlast(t_elem *lst)
{
	t_elem	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	if (ptr && ptr->next)
	{
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
	}
	return (ptr);
}

t_env	*ft_lstlast2(t_env *lst)
{
	t_env	*ptr;

	if (!lst)
		return (NULL);
	ptr = lst;
	if (ptr && ptr->next)
	{
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
	}
	return (ptr);
}

int	ft_lstsize(t_elem *lst)
{
	int	count;

	count = 0;
	if (!lst)
		return (0);
	while (lst != NULL)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

int	ft_strlen(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
