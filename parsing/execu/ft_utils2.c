/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:36:35 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/18 04:00:26 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_lstsizeee(t_env *lst)
{
	t_env	*tmp;
	int		i;

	tmp = lst;
	i = 0;
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

t_env	*ft_lstlasttt(t_env *lst)
{
	t_env	*node;

	node = lst;
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}

void	ft_lstadd_backkk(t_env **lst, t_env *new)
{
	t_env	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlasttt(*lst);
	last->next = new;
}

t_env	*ft_lstnewww(char *key, char *val)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = val;
	node->flag = 0;
	node->next = NULL;
	return (node);
}

char	*ft_strduppp(char *s1)
{
	size_t	i;
	size_t	len;
	char	*s2;

	i = 0;
	len = ft_strlen(s1) + 1;
	s2 = (char *)malloc(len);
	if (!s2)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
