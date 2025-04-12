/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:34:24 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:49:35 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos(t_table *table, int i, char *argv[])
{
	table->philos[i].id = i;
	table->philos[i].lfork = &table->forks[i];
	table->philos[i].rfork = &table->forks[(i + 1) % table->num_philos];
	table->philos[i].start_time = get_time();
	table->philos[i].last_meal = table->philos[i].start_time;
	table->philos[i].eat_count = 0;
	table->philos[i].time_to_die = to_int(argv[2]);
	table->philos[i].time_to_eat = to_int(argv[3]);
	table->philos[i].time_to_sleep = to_int(argv[4]);
	table->philos[i].print_lock = &table->print_lock;
	table->philos[i].num_philos = table->num_philos;
	table->philos[i].table = table;
}

void	init_table(t_table *table, int argc, char *argv[])
{
	int	i;

	table->died = 0;
	table->all_ate = 0;
	table->num_philos = to_int(argv[1]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = to_int(argv[5]);
	i = 0;
	while (i < table->num_philos)
		pthread_mutex_init(&table->forks[i++], NULL);
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->eat_count_lock, NULL);
	pthread_mutex_init(&table->last_meal_lock, NULL);
	i = 0;
	while (i < table->num_philos)
		init_philos(table, i++, argv);
}

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->lfork);
	philo_print(philo->print_lock, philo, "has taken a fork");
	ft_usleep(philo->time_to_die);
	pthread_mutex_unlock(philo->lfork);
	return (NULL);
}

void	create_philo(t_table *table)
{
	int	i;

	i = 1;
	while (i < table->num_philos)
	{
		pthread_create(&table->philos[i].thread, NULL, philosopher,
			&table->philos[i]);
		i += 2;
	}
	i = 0;
	while (i < table->num_philos - 1)
	{
		pthread_create(&table->philos[i].thread, NULL, philosopher,
			&table->philos[i]);
		i += 2;
	}
	if (table->num_philos % 2 == 1)
		pthread_create(&table->philos[table->num_philos - 1].thread, NULL,
			philosopher, &table->philos[table->num_philos - 1]);
}
