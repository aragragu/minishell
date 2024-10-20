/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 18:45:01 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:53:38 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_elem	*ft_lstnew(void *content, t_token type, t_garbage **garbage)
{
	t_elem	*new_node;

	new_node = (t_elem *)malloc(sizeof(t_elem));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->content = content;
	new_node->type = type;
	new_node->fd = -1;
	new_node->fd_here = 0;
	new_node->ignore[0] = 0;
	new_node->ignore[1] = 0;
	new_node->next = NULL;
	return (new_node);
}

t_env	*ft_lstnew2(char *key, char *value, t_garbage **garbage)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->key = key;
	new_node->value = value;
	new_node->flag = 0;
	new_node->next = NULL;
	return (new_node);
}

t_herdoc	*ft_lstnew3(t_elem **list, char *buffer, t_garbage **garbage)
{
	t_herdoc	*new_node;

	new_node = malloc(sizeof(t_herdoc));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->current = *list;
	new_node->buffer = buffer;
	return (new_node);
}

t_redir	*ft_lstnew_redi(char *value, t_token type,	int fd, t_garbage **garbage)
{
	t_redir	*new_node;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->value = value;
	new_node->type = type;
	new_node->fd = fd;
	new_node->next = NULL;
	return (new_node);
}

t_garbage	*ft_lstnew_garbage(void *content)
{
	t_garbage	*new_node;

	new_node = (t_garbage *)malloc(sizeof(t_garbage));
	if (!new_node)
		return (NULL);
	new_node->value = content;
	new_node->next = NULL;
	return (new_node);
}
