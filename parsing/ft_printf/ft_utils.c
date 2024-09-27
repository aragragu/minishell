/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:18 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/27 14:57:57 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putnbr(int n)
{
	long	nb;
	int		cont;

	nb = n;
	cont = 0;
	if (nb < 0)
	{
		ft_putchar('-');
		nb = -nb;
		cont++;
	}
	if (nb >= 10)
	{
		cont += ft_putnbr(nb / 10);
		cont += ft_putnbr(nb % 10);
	}
	else
	{
		nb = nb + 48;
		ft_putchar(nb);
		cont++;
	}
	return (cont);
}

int	ft_putnbr_hexa(unsigned long n, int a)
{
	int		cont;

	cont = 0;
	if (n >= 16)
	{
		cont += ft_putnbr_hexa(n / 16, a);
	}
	if (n % 16 < 10)
	{
		cont += ft_putchar((n % 16) + 48);
	}
	else
	{
		if (a == 1)
			cont += ft_putchar((n % 16) + 87);
		else
			cont += ft_putchar((n % 16) + 55);
	}
	return (cont);
}

int	ft_putnbr_unsd(unsigned int n)
{
	unsigned long	nb;
	int				cont;

	nb = n;
	cont = 0;
	if (nb >= 10)
	{
		cont += ft_putnbr(nb / 10);
		cont += ft_putnbr(nb % 10);
	}
	else
	{
		nb = nb + 48;
		ft_putchar(nb);
		cont++;
	}
	return (cont);
}

int	ft_putstr2(char *s)
{
	if (!s)
		return (ft_putstr2("(null)"));
	return (write(1, s, ft_strlen(s)));
}
