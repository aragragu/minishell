/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:18 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/28 15:16:14 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_putnbr_fd(int n, int fd)
{
	long	nb;
	int		cont;

	nb = n;
	cont = 0;
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
		cont++;
	}
	if (nb >= 10)
	{
		cont += ft_putnbr_fd(nb / 10, fd);
		cont += ft_putnbr_fd(nb % 10, fd);
	}
	else
	{
		nb = nb + 48;
		ft_putchar_fd(nb, fd);
		cont++;
	}
	return (cont);
}

int	ft_putnbr_hexa_fd(unsigned long n, int a, int fd)
{
	int		cont;

	cont = 0;
	if (n >= 16)
	{
		cont += ft_putnbr_hexa_fd(n / 16, a, fd);
	}
	if (n % 16 < 10)
	{
		cont += ft_putchar_fd((n % 16) + 48, fd);
	}
	else
	{
		if (a == 1)
			cont += ft_putchar_fd((n % 16) + 87, fd);
		else
			cont += ft_putchar_fd((n % 16) + 55, fd);
	}
	return (cont);
}

int	ft_putnbr_unsd_fd(unsigned int n, int fd)
{
	unsigned long	nb;
	int				cont;

	nb = n;
	cont = 0;
	if (nb >= 10)
	{
		cont += ft_putnbr_fd(nb / 10, fd);
		cont += ft_putnbr_fd(nb % 10, fd);
	}
	else
	{
		nb = nb + 48;
		ft_putchar_fd(nb, fd);
		cont++;
	}
	return (cont);
}

int	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return (ft_putstr_fd("(null)", fd));
	return (write(fd, s, strlen(s)));
}
