/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:28:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:29:30 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philosopher(void *arg)
{
    t_philo *philo;
    int should_continue;

    philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
        philo_print(philo->print_lock, philo, "is thinking");
    while (1)
    {
        pthread_mutex_lock(&philo->table->print_lock);
        should_continue = (philo->table->died == 0 && philo->table->all_ate == 0);
        pthread_mutex_unlock(&philo->table->print_lock);
        if (!should_continue)
            break;
		get_fork_and_eat(philo);
        philo_print(philo->print_lock, philo, "is sleeping");
        ft_usleep(philo->time_to_sleep);
        philo_print(philo->print_lock, philo, "is thinking");
        if (philo->num_philos % 2 == 1)
            ft_usleep(max(0, 2 * philo->time_to_eat - philo->time_to_sleep));
    }
    return (NULL);
}

void *check_time(t_table *table,int i)
{
	long timestamp;

	timestamp = get_time();
	if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
	{
		pthread_mutex_lock(&table->print_lock);
		table->died = 1;
		printf("%ld %d died\n", get_time() - table->philos[i].start_time, i + 1);
		pthread_mutex_unlock(&table->print_lock);
		pthread_mutex_unlock(&table->last_meal_lock);
		return (NULL);
	}
}

void *death_check(void *arg)
{
    t_table *table;
    int i;
    int should_continue;

    table = (t_table *)arg;
    while (1)
    {
        pthread_mutex_lock(&table->print_lock);
        should_continue = (table->all_ate == 0 && table->died == 0);
        pthread_mutex_unlock(&table->print_lock);
        if (!should_continue)
            break;
        i = -1;
        while (++i < table->num_philos)
        {
            pthread_mutex_lock(&table->last_meal_lock);
            if(check_time(table,i) == NULL)
				return NULL;
            pthread_mutex_unlock(&table->last_meal_lock);
        }
        usleep(1000);
    }
    return (NULL);
}

void check_eat_count(t_table *table, int *all_ate)
{
	int i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_lock(&table->eat_count_lock);
		if (table->must_eat_count == -1 || table->philos[i].eat_count < table->must_eat_count)
		{
			*all_ate = 0;
			pthread_mutex_unlock(&table->eat_count_lock);
			break;
		}
		pthread_mutex_unlock(&table->eat_count_lock);
		i++;
	}
}

void *eat_check(void *arg)
{
    t_table *table;
    int all_ate;
    int should_continue;

    table = (t_table *)arg;
    while (1)
    {
        pthread_mutex_lock(&table->print_lock);
        should_continue = (table->died == 0);
        pthread_mutex_unlock(&table->print_lock);
        if (!should_continue)
            break;   
        all_ate = 1;
        check_eat_count(table, &all_ate);
        if (all_ate)
        {
            pthread_mutex_lock(&table->print_lock);
            table->all_ate = 1;
            pthread_mutex_unlock(&table->print_lock);
            return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}
