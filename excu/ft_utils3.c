/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 18:46:15 by ykasmi            #+#    #+#             */
/*   Updated: 2024/08/24 17:33:52 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*ss;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	ss = (char *)malloc (ft_strlen (s1) + ft_strlen (s2) + 1);
	if (ss == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		ss[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != '\0')
	{
		ss[j++] = s2[i++];
	}
	ss[j] = '\0';
	return (free(s1), ss);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}
