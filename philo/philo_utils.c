/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:36:14 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:29:36 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000L + time.tv_usec / 1000);
}

int ft_usleep(long time)
{
    long start;

    start = get_time();
    while ((get_time() - start) < time)
    {
        if ((get_time() - start) < time - 5)
            usleep(500);
        else
            usleep(50);
    }
    return 0;
}

void philo_print(pthread_mutex_t *print_lock, t_philo *philo, char *message)
{
    long timestamp;

    pthread_mutex_lock(print_lock);
    if (philo->table->died || philo->table->all_ate)
    {
        pthread_mutex_unlock(print_lock);
        return;
    }
    timestamp = get_time() - philo->start_time;
    printf("%ld %d %s\n", timestamp, philo->id + 1, message);
    pthread_mutex_unlock(print_lock);
}

void get_fork(t_philo *philo)
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
	pthread_mutex_lock(&philo->table->last_meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->table->last_meal_lock);
	philo_print(philo->print_lock, philo, "is eating");
	ft_usleep(philo->time_to_eat);
	pthread_mutex_lock(&philo->table->eat_count_lock);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->table->eat_count_lock);
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
}
