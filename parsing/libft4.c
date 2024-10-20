/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 20:51:29 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/20 20:51:49 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen2(char **str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != NULL)
		i++;
	return (i);
}

char	*ft_substr(char *s, int start, int len, t_garbage **garbage)
{
	char	*ptr;

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

int	ft_strlcpy(char *dest, char *src, int size)
{
	int	i;

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

void	*ft_calloc(int count, int size, t_garbage **garbage)
{
	void	*data;

	if (count > 0 && (unsigned long long)size > SIZE_MAX
		/ (unsigned long long)count)
		return (0);
	data = malloc(count * size);
	if (!data)
		return (NULL);
	ft_bzero(data, (count * size));
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(data));
	return (data);
}

char	*ft_strdup(char *s1, t_garbage **garbage)
{
	int		z;
	char	*ptr;

	z = 0;
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!ptr)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(ptr));
	while (s1 && s1[z])
	{
		ptr[z] = s1[z];
		z++;
	}
	ptr[z] = '\0';
	return (ptr);
}
