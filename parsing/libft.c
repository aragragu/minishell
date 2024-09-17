/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 18:45:01 by aragragu          #+#    #+#             */
/*   Updated: 2024/09/17 17:41:46 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_elem *ft_lstnew(void *content, t_token type, t_garbage **garbage)
{
	t_elem *new_node;

	new_node = (t_elem *)malloc(sizeof(t_elem));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->content = content;
	new_node->type = type;
	new_node->next = NULL;
	return (new_node);
}

t_env *ft_lstnew2(char *key, char *value, t_garbage **garbage)
{
	t_env *new_node;

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
t_redir *ft_lstnew_redi(char *value, t_token type, t_garbage **garbage)
{
	t_redir *new_node;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(new_node));
	new_node->value = value;
	new_node->type = type;
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


void ft_lstadd_back(t_elem **lst, t_elem *new)
{
	if (!lst || !new)
        return;
    
    if (*lst == NULL)
    {
        *lst = new;
    }
    else
    {
        t_elem *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}

void ft_lstadd_back2(t_env **lst, t_env *new)
{
    if (!lst || !new)
        return;
    
    if (*lst == NULL)
        *lst = new;
    else
    {
        t_env *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}

void ft_lstadd_back_garbage(t_garbage **lst, t_garbage *new)
{
    if (!lst || !new)
        return;
    
    if (*lst == NULL)
        *lst = new;
    else
    {
        t_garbage *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}
void ft_lstadd_back_redi(t_redir **lst, t_redir *new)
{
    if (!lst || !new)
        return;
    
    if (*lst == NULL)
        *lst = new;
    else
    {
        t_redir *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}

void ft_lstadd_back_cmd(t_cmd **lst, t_cmd *new)
{
    if (!lst || !new)
        return;
    
    if (*lst == NULL)
        *lst = new;
    else
    {
        t_cmd *ptr = *lst;
        while (ptr->next)
            ptr = ptr->next;
        ptr->next = new;
    }
}


t_elem *ft_lstlast(t_elem *lst)
{
	t_elem *ptr;
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

t_env *ft_lstlast2(t_env *lst)
{
	t_env *ptr;
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



int ft_lstsize(t_elem *lst)
{
	int count;

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

int ft_strlen(char *str)
{
	int i;
	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}
int ft_strlen2(char **str)
{
	int i;
	if (!str)
		return (0);
	i = 0;
	while (str[i] != NULL)
		i++;
	return (i);
}

char *ft_substr(char *s, int start, int len, t_garbage **garbage)
{
	char *ptr;

	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start)
		return (ft_calloc(sizeof(char), 1, garbage));
	if (len >= (ft_strlen(s) - start))
		return (ft_strdup(s + start, garbage));
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(ptr));
	ft_strlcpy(ptr, s + start, len + 1);
	return (ptr);
}

int ft_strlcpy(char *dest, char *src, int size)
{
	int i;

	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen(src));
}

void *ft_calloc(int count, int size, t_garbage **garbage)
{
	void *data;

	if (count > 0 && (unsigned long long)size > SIZE_MAX / (unsigned long long)count)
		return (0);
	data = malloc(count * size);
	if (!data)
		return (NULL);
	ft_bzero(data, (count * size));
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(data));
	return (data);
}

char *ft_strdup(char *s1, t_garbage **garbage)
{
	int z;
	char *ptr;

	z = 0;
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!ptr)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(ptr));
	while (s1[z])
	{
		ptr[z] = s1[z];
		z++;
	}
	ptr[z] = '\0';
	return (ptr);
}

void ft_bzero(void *s, int n)
{
	int i;
	char *ptr;

	i = 0;
	ptr = s;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

char *ft_strtrim(char *s1, char *set, t_garbage **garbage)
{
	size_t start;
	size_t end;
	char *ptr;

	if (s1 == NULL)
		return (NULL);
	if (s1[0] == '\0')
		return (ft_strdup("", garbage));
	if (s1 == 0 || set == 0)
		return (NULL);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	while (s1[end - 1] && ft_strchr(set, s1[end - 1]) && end > start)
		end--;
	ptr = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!ptr)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(ptr));
	ft_strlcpy(ptr, s1 + start, end - start + 1);
	return (ptr);
}

char *ft_strchr(char *s, int c)
{
	while (*s != (unsigned char)c)
	{
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return ((char *)s);
}

int	ft_strcmp(char *s1, char *s2)
{
	int i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

char		*ft_strjoin(char *s1, char *s2, t_garbage **garbage)
{
	char			*ptr;
	size_t			i;
	size_t			y;

	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!ptr)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(ptr));
	i = 0;
	y = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[y])
	{
		ptr[i++] = s2[y++];
	}
	ptr[i] = '\0';
	return (ptr);
}