/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:34 by ykasmi            #+#    #+#             */
/*   Updated: 2024/10/05 19:20:28 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_fd(char c, va_list ap, int *s, int fd)
{
	if (c == '%')
		*s += ft_putchar_fd('%', fd);
	else if (c == 'd' || c == 'i')
		*s += ft_putnbr_fd(va_arg(ap, int), fd);
	else if (c == 's')
		*s += ft_putstr_fd(va_arg(ap, char *), fd);
	else if (c == 'c')
		*s += ft_putchar_fd(va_arg(ap, int), fd);
	else if (c == 'x')
		*s += ft_putnbr_hexa_fd(va_arg(ap, unsigned int), 1, fd);
	else if (c == 'X')
		*s += ft_putnbr_hexa_fd(va_arg(ap, unsigned int), 0, fd);
	else if (c == 'u')
		*s += ft_putnbr_unsd_fd(va_arg(ap, unsigned int), fd);
	else if (c == 'p')
	{
		*s += ft_putstr_fd("0x", fd);
		*s += ft_putnbr_hexa_fd(va_arg(ap, unsigned long), 1, fd);
	}
	else
		*s += ft_putchar_fd(c, fd);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	int		i;
	va_list	ap;

	i = 0;
	va_start(ap, format);
	if (write(fd, "", 0) < 0)
		return (-1);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '\0')
				return (i);
			print_fd(*format, ap, &i, fd);
		}
		else
		{
			ft_putchar_fd(*format, fd);
			i++;
		}
		format++;
	}
	va_end(ap);
	return (i);
}
