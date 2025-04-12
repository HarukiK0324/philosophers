/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 19:24:23 by hkasamat         ###   ########.fr       */
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

void *death_check(void *arg)
{
    t_table *table;
    int i;
    long timestamp;
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
            timestamp = get_time();
            pthread_mutex_lock(&table->last_meal_lock);
            if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
            {
                pthread_mutex_lock(&table->print_lock);
                table->died = 1;
                printf("%ld %d died\n", get_time() - table->philos[i].start_time, i + 1);
                pthread_mutex_unlock(&table->print_lock);
                pthread_mutex_unlock(&table->last_meal_lock);
                return (NULL);
            }
            pthread_mutex_unlock(&table->last_meal_lock);
        }
        usleep(1000);
    }
    return (NULL);
}

void *eat_check(void *arg)
{
    t_table *table;
    int i;
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
        i = -1;
        while (++i < table->num_philos)
        {
            pthread_mutex_lock(&table->eat_count_lock);
            if (table->must_eat_count == -1 || table->philos[i].eat_count < table->must_eat_count)
            {
                all_ate = 0;
                pthread_mutex_unlock(&table->eat_count_lock);
                break;
            }
            pthread_mutex_unlock(&table->eat_count_lock);
        }
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

int main(int argc, char *argv[])
{
    int i;
    t_table table;

    if ((argc != 5 && argc != 6) || invalid_args(argc, argv))
    {
        printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (0);
    }    
    init_table(&table, argc, argv);
    if (table.num_philos == 1)
        pthread_create(&table.philos[0].thread, NULL, one_philo, &table.philos[0]);
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
