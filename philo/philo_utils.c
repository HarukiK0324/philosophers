/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:36:14 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 18:47:54 by hkasamat         ###   ########.fr       */
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
