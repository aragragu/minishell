/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:35:50 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/28 15:33:21 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// void	ft_putstr_fd(char *s, int fd)
// {
// 	int	i;

// 	i = 0;
// 	if (fd < 0)
// 		return ;
// 	if (!s)
// 		return ;
// 	while (s[i] != '\0')
// 	{
// 		write(fd, &s[i], 1);
// 		i++;
// 	}
// }

char	*ft_strjoinnn(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*ss;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	ss = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
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
