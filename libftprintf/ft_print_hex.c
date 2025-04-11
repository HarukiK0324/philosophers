/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:16:47 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/11 09:45:12 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	hex_size(unsigned int num)
{
	int	i;

	i = 1;
	while (num >= 16)
	{
		num /= 16;
		i++;
	}
	return (i);
}

int	ft_print_hex_lower(unsigned int num,char c)
{
	if (num >= 16)
		ft_print_hex_lower(num / 16,c);
	if (num % 16 >= 10)
		ft_print_char(num % 16 + c - 10);
	else
		ft_print_char(num % 16 + '0');
	return (hex_size(num));
}
