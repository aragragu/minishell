/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 16:55:29 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/03 19:55:37 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(char *str, int c)
{
	unsigned int	i;
	char			c1;

	c1 = (char)c;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c1)
			return (str + i);
		i++;
	}
	if (str[i] == c1)
	{
		return (str + i);
	}
	return (NULL);
}

int	ft_strcmp(char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return(NULL);
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_isalpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	ft_digits(char c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char	*ft_strdup(char *s1)
{
	size_t	i;
	size_t	len;
	char	*s2;

	i = 0;
	len = ft_strlen (s1) + 1;
	s2 = (char *)malloc(len);
	if (!s2)
		return (NULL);
	while (s1[i] != '\0')
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
