/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aragragu <aragragu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:51:26 by aragragu          #+#    #+#             */
/*   Updated: 2024/10/07 20:51:28 by aragragu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	len_nem(long nb)
{
	int			i;

	i = 0;
	if (nb <= 0)
	{
		nb *= -1;
		++i;
	}
	while (nb > 0)
	{
		++i;
		nb /= 10;
	}
	return (i);
}

char	*ft_itoa(int nb, t_garbage **garbage)
{
	long		xd;
	char		*num;
	int			i;

	xd = (long)nb;
	i = len_nem(xd);
	num = (char *)malloc(sizeof(char) * (i + 1));
	if (!num)
		return (NULL);
	ft_lstadd_back_garbage(garbage, ft_lstnew_garbage(num));
	num[i] = '\0';
	if (xd == 0)
		num[0] = '0';
	else if (xd < 0)
	{
		num[0] = '-';
		xd *= -1;
	}
	while (xd != 0)
	{
		--i;
		num[i] = (xd % 10) + 48;
		xd /= 10;
	}
	return (num);
}
