/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_num.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:16:47 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/11 09:44:59 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_digits(int num)
{
	int	i;

	i = 1;
	if (num < 0)
	{
		num *= -1;
		i++;
	}
	while (num >= 10)
	{
		num /= 10;
		i++;
	}
	return (i);
}

int	ft_print_num(int num)
{
	int	i;

	i = 0;
	if (num == -2147483648)
		return(ft_print_string("-2147483648"));
	else if (num < 0)
	{
		ft_print_char('-');
		num *= -1;
		i++;
	}
	if (num >= 10)
		ft_print_num(num / 10);
	ft_print_char(num % 10 + '0');
	return (i + ft_digits(num));
}
