/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:28:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 21:00:18 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *arg)
{
	int		check;
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		philo_print(philo->print_lock, philo, "is thinking");
	while (1)
	{
		pthread_mutex_lock(&philo->table->print_lock);
		check = (philo->table->died == 0 && philo->table->all_ate == 0);
		pthread_mutex_unlock(&philo->table->print_lock);
		if (!check)
			break ;
		get_fork(philo);
		philo_print(philo->print_lock, philo, "is sleeping");
		ft_usleep(philo->time_to_sleep);
		philo_print(philo->print_lock, philo, "is thinking");
		if (philo->num_philos % 2 == 1)
			ft_usleep(max(0, 2 * philo->time_to_eat - philo->time_to_sleep));
	}
	return (NULL);
}

int	check_time(t_table *table, int i)
{
	long	timestamp;

	timestamp = get_time();
	if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
	{
		pthread_mutex_lock(&table->print_lock);
		table->died = 1;
		printf("%ld %d died\n", get_time() - table->philos[i].start_time, i
			+ 1);
		pthread_mutex_unlock(&table->print_lock);
		pthread_mutex_unlock(&table->last_meal_lock);
		return (-1);
	}
	return (0);
}

void	*death_check(void *arg)
{
	t_table	*table;
	int		i;
	int		check;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->print_lock);
		check = (table->all_ate == 0 && table->died == 0);
		pthread_mutex_unlock(&table->print_lock);
		if (!check)
			break ;
		i = -1;
		while (++i < table->num_philos)
		{
			pthread_mutex_lock(&table->last_meal_lock);
			if (check_time(table, i) == -1)
				return (NULL);
			pthread_mutex_unlock(&table->last_meal_lock);
		}
	}
	return (NULL);
}

void	check_eat_count(t_table *table, int *all_ate)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_lock(&table->eat_count_lock);
		if (table->must_eat_count == -1
			|| table->philos[i].eat_count < table->must_eat_count)
		{
			*all_ate = 0;
			pthread_mutex_unlock(&table->eat_count_lock);
			break ;
		}
		pthread_mutex_unlock(&table->eat_count_lock);
		i++;
	}
}

void	*eat_check(void *arg)
{
	t_table	*table;
	int		all_ate;
	int		check;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->print_lock);
		check = (table->died == 0);
		pthread_mutex_unlock(&table->print_lock);
		if (!check)
			break ;
		all_ate = 1;
		check_eat_count(table, &all_ate);
		if (all_ate)
		{
			pthread_mutex_lock(&table->print_lock);
			table->all_ate = 1;
			pthread_mutex_unlock(&table->print_lock);
			return (NULL);
		}
	}
	return (NULL);
}
