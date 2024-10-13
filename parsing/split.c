/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:38:29 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/12 18:32:32 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sign(char c, int *sign)
{
	if (c == '"' && *sign == 2)
		*sign = 0;
	else if (c == '\'' && *sign == 1)
		*sign = 0;
	else if (c == '"' && *sign == 0)
		*sign = 2;
	else if (c == '\'' && *sign == 0)
		*sign = 1;
}

static	int	count_words(const char *s, char c)
{
	size_t		count;
	const char	*ptr;

	count = 0;
	ptr = s;
	while (*ptr)
	{
		while (*ptr == c)
			ptr++;
		if (*ptr)
		{
			while (*ptr && *ptr != c)
				ptr++;
			count++;
		}
	}
	return (count);
}

static	size_t	count_letter(const char *s, int position, char c)
{
	size_t		i;

	i = 0;
	while (*(s + position + i) && *(s + position + i) != c)
		i++;
	return (i);
}

static void	ft_free1(char **reslut, int len)
{
	int		i;

	i = 0;
	while (i < len)
	{
		free(reslut[i]);
		i++;
	}
	free(reslut);
}

static	char	**allocation(char **s, const char *x, char c,
t_garbage **garbage)
{
	int		z;
	int		y;
	int		i;

	i = 0;
	y = 0;
	while (x[i])
	{
		z = 0;
		while (x[i] && x[i] == c)
			++i;
		if (x[i])
		{
			s[y] = (char *)ft_calloc((count_letter(x, i, c) + 1),
					sizeof(char), garbage);
			if (!s[y])
			{
				ft_free1(s, y);
				return (NULL);
			}
			while (x[i] && x[i] != c)
				s[y][z++] = x[i++];
			y++;
		}
	}
	return (s);
}

char	**ft_split(char const *s, char c, t_garbage **garbage)
{
	char		**result;
	size_t		count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	result = (char **)ft_calloc((count + 1), sizeof(char *), garbage);
	if (!result)
		return (NULL);
	result[count] = NULL;
	result = allocation(result, s, c, garbage);
	return (result);
}
