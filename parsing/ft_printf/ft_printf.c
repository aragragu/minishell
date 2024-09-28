/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykasmi <ykasmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:34 by ykasmi            #+#    #+#             */
/*   Updated: 2024/09/27 14:59:06 by ykasmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print(char c, va_list ap, int *s)
{
	
	if (c == '%')
		*s += ft_putchar('%');
	else if (c == 'd' || c == 'i')
		*s += ft_putnbr(va_arg(ap, int));
	else if (c == 's')
		*s += ft_putstr2(va_arg(ap, char *));
	else if (c == 'c')
		*s += ft_putchar(va_arg(ap, int));
	else if (c == 'x')
		*s += ft_putnbr_hexa(va_arg(ap, unsigned int), 1);
	else if (c == 'X')
		*s += ft_putnbr_hexa(va_arg(ap, unsigned int), 0);
	else if (c == 'u')
		*s += ft_putnbr_unsd(va_arg(ap, unsigned int));
	else if (c == 'p')
	{
		*s += ft_putstr2("0x");
		*s += ft_putnbr_hexa(va_arg(ap, unsigned long), 1);
	}
	else
		*s += ft_putchar(c);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	ap;

	i = 0;
	va_start(ap, format);
	if (write(1, "", 0) < 0)
		return (-1);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (*format == '\0')
				return (i);
			print(*format, ap, &i);
		}
		else
		{
			ft_putchar(*format);
			i++;
		}
		format++;
	}
	va_end (ap);
	return (i);
}
