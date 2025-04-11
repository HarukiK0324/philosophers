/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:46:57 by haruki            #+#    #+#             */
/*   Updated: 2025/04/11 09:46:21 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_specifier(char format, va_list args)
{
	if (format == 'c')
		return (ft_print_char(va_arg(args, int)));
	else if (format == 's')
		return (ft_print_string(va_arg(args, char *)));
	else if (format == 'p')
		return (ft_print_pointer(va_arg(args, void *)));
	else if (format == 'd' || format == 'i')
		return (ft_print_num(va_arg(args, int)));
	else if (format == 'u')
		return (ft_print_unsigned_num(va_arg(args, unsigned int)));
	else if (format == 'x')
		return (ft_print_hex(va_arg(args, unsigned int),'a'));
	else if (format == 'X')
		return (ft_print_hex(va_arg(args, unsigned int),'A'));
	else if (format == '%')
		return (ft_print_char('%'));
	else
		return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		result;

	va_start(args, (char *)format);
	i = 0;
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			if (*format != '\0')
			{
				result = handle_specifier(*format, args);
				if (result == -1)
					return (-1);
				i += result;
			}
		}
		else
			i += ft_print_char(*format);
		format++;
	}
	va_end(args);
	return (i);
}
