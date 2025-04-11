/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_num.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haruki <haruki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:16:47 by hkasamat          #+#    #+#             */
/*   Updated: 2024/12/14 17:57:10 by haruki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_unsigned_digits(unsigned int num)
{
	int	i;

	i = 1;
	while (num >= 10)
	{
		num /= 10;
		i++;
	}
	return (i);
}

int	ft_print_unsigned_num(unsigned int num)
{
	if (num >= 10)
	{
		ft_print_unsigned_num(num / 10);
		ft_print_char(num % 10 + '0');
	}
	else
		ft_print_char(num + '0');
	return (ft_unsigned_digits(num));
}
