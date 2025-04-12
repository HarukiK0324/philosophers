/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:49:37 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	int		i;
	t_table	table;

	if ((argc != 5 && argc != 6) || invalid_args(argc, argv))
	{
		printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
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
	pthread_mutex_destroy(&table.eat_count_lock);
	pthread_mutex_destroy(&table.last_meal_lock);
	return (0);
}
