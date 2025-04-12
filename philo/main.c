/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 19:03:10 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		philo_print(philo->print_lock, philo, "is thinking");
	while (philo->table->died == 0 && philo->table->all_ate == 0)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->rfork);
			philo_print(philo->print_lock, philo, "has taken a fork");
			pthread_mutex_lock(philo->lfork);
			philo_print(philo->print_lock, philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->lfork);
			philo_print(philo->print_lock, philo, "has taken a fork");
			pthread_mutex_lock(philo->rfork);
			philo_print(philo->print_lock, philo, "has taken a fork");
		}
		pthread_mutex_lock(&philo->table->eat_count_lock);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->table->eat_count_lock);
		philo_print(philo->print_lock, philo, "is eating");
		ft_usleep(philo->time_to_eat);
		pthread_mutex_lock(&philo->table->eat_count_lock);
		philo->eat_count++;
		pthread_mutex_unlock(&philo->table->eat_count_lock);
		philo_print(philo->print_lock, philo, "is sleeping");
		pthread_mutex_unlock(philo->lfork);
		pthread_mutex_unlock(philo->rfork);
		ft_usleep(philo->time_to_sleep);
		philo_print(philo->print_lock, philo, "is thinking");
		if (philo->num_philos % 2 == 1)
			ft_usleep(max(0, 2 * philo->time_to_eat - philo->time_to_sleep));
	}
	return (NULL);
}

void	*death_check(void *arg)
{
	t_table	*table;
	int		i;
	long	timestamp;

	table = (t_table *)arg;
	while (table->all_ate == 0)
	{
		i = -1;
		while (++i < table->num_philos)
		{
			timestamp = get_time();
			pthread_mutex_lock(&table->last_meal_lock);
			if (timestamp
				- table->philos[i].last_meal > table->philos[i].time_to_die)
			{
				table->died = 1;
				pthread_mutex_lock(&table->print_lock);
				printf("%ld %d died\n", get_time()
					- table->philos[i].start_time, i + 1);
				pthread_mutex_unlock(&table->print_lock);
				pthread_mutex_unlock(&table->last_meal_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&table->last_meal_lock);
		}
	}
	return (NULL);
}

void	*eat_check(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (table->died == 0)
	{
		i = -1;
		while (++i < table->num_philos)
		{
			pthread_mutex_lock(&table->eat_count_lock);
			if (table->philos[i].eat_count < table->must_eat_count)
			{
				pthread_mutex_unlock(&table->eat_count_lock);
				break ;
			}
			pthread_mutex_unlock(&table->eat_count_lock);
		}
		if (i == table->num_philos && table->died == 0)
		{
			pthread_mutex_lock(&table->print_lock);
			table->all_ate = 1;
			pthread_mutex_unlock(&table->print_lock);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_table	table;

	if ((argc != 5 && argc != 6) || invalid_args(argc, argv))
		return (printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"),
			0);
	init_table(&table, argc, argv);
	if (table.num_philos == 1)
		pthread_create(&table.philos[0].thread, NULL, one_philo,
			&table.philos[0]);
	else
		create_philo(&table);
	pthread_create(&table.death_thread, NULL, death_check, &table);
	if (table.must_eat_count != -1)
		pthread_create(&table.eat_thread, NULL, eat_check, &table);
	i = 0;
	while (i < table.num_philos)
		pthread_join(table.philos[i++].thread, NULL);
	pthread_join(table.death_thread, NULL);
	if (table.must_eat_count != -1)
		pthread_join(table.eat_thread, NULL);
	i = 0;
	while (i < table.num_philos)
		pthread_mutex_destroy(&table.forks[i++]);
	pthread_mutex_destroy(&table.print_lock);
	return (0);
}
