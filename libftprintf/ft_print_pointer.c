/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 17:30:19 by haruki            #+#    #+#             */
/*   Updated: 2025/04/11 09:44:32 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pointer_digits(unsigned long ptr)
{
	int	i;

	i = 1;
	while (ptr >= 16)
	{
		ptr /= 16;
		i++;
	}
	return (i);
}

void	ft_print_hex_pointer(unsigned long ptr)
{
	if (ptr >= 16)
		ft_print_hex_pointer(ptr / 16);
	if (ptr % 16 >= 10)
		ft_print_char(ptr % 16 + 'a' - 10);
	else
		ft_print_char(ptr % 16 + '0');
}

int	ft_print_pointer(void *ptr)
{
	unsigned long	address;

	address = (unsigned long)ptr;
	if (ptr == NULL)
		return(ft_print_string("(nil)"));
	ft_print_string("0x");
	ft_print_hex_pointer(address);
	return (ft_pointer_digits(address) + 2);
}
