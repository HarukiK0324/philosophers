/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:33:24 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:50:45 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	str_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

int	to_int(char *str)
{
	int		i;
	int		itr;
	long	num;

	num = 0;
	i = 0;
	itr = 1;
	if (str_len(str) > 10)
		return (-1);
	if (str[0] == '-')
	{
		itr = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		num = num * 10 + itr * (str[i] - '0');
		i++;
	}
	if (num > INT_MAX)
		return (-1);
	return (num);
}

int	check_num(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	return (0);
}

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	invalid_args(int argc, char *argv[])
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (to_int(argv[i]) <= 0 || check_num(argv[i]) == -1)
			return (1);
		i++;
	}
	if (to_int(argv[1]) > MAX_PHILOS)
		return (1);
	else if (to_int(argv[2]) < 60 || to_int(argv[3]) < 60
		|| to_int(argv[4]) < 60)
		return (1);
	return (0);
}
