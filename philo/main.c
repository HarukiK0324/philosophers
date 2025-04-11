/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/11 11:56:16 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time_in_ms(void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000L + time.tv_usec / 1000);
}

int ft_usleep(long time)
{
    long start;

    start = get_time_in_ms();
    while ((get_time_in_ms() - start) < time)
    {
        if ((get_time_in_ms() - start) < time - 5)
            usleep(500);
        else
            usleep(50);
    }
    return 0;
}

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

void philo_print(pthread_mutex_t *print_lock, t_philo *philo, char *message)
{
    long timestamp;
    
    pthread_mutex_lock(print_lock);
    timestamp = get_time_in_ms() - philo->start_time;
    if(philo->died ==0)
        printf("%ld %d %s\n",timestamp, philo->id + 1, message);
    pthread_mutex_unlock(print_lock);
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    if(philo->id % 2 == 0)
        philo_print(philo->print_lock, philo, "is thinking");
    while (philo->died == 0)
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
        philo_print(philo->print_lock, philo, "is eating");
        philo->last_meal = get_time_in_ms() - philo->start_time;
        ft_usleep(philo->time_to_eat);
        philo->times_ate++;
        philo_print(philo->print_lock, philo, "is sleeping");
        philo->last_meal = get_time_in_ms() - philo->start_time;
        pthread_mutex_unlock(philo->lfork);
        pthread_mutex_unlock(philo->rfork);
        ft_usleep(philo->time_to_sleep);
        philo_print(philo->print_lock, philo, "is thinking");
        if(philo->num_philos%2 == 1)
            ft_usleep(max(0,2*philo->time_to_eat-philo->time_to_sleep));
    }
    return NULL;
}

int invalid_args(int argc, char *argv[])
{
   int i;

   i = 1;
   while (i < argc)
   {
      if (to_int(argv[i]) <= 0 || check_num(argv[i]) == -1)
         return (1);
      i++;
   }
   if(to_int(argv[1]) > 200)
      return (1);
   else if(to_int(argv[2]) < 60 || to_int(argv[3]) < 60 || to_int(argv[4]) < 60)
      return (1);
   return (0);
}

void init_philos(t_table *table, int i,char *argv[])
{
   table->philos[i].id = i;
   table->philos[i].lfork = &table->forks[i];
   table->philos[i].rfork = &table->forks[(i + 1) % table->num_philos];
   table->philos[i].start_time = get_time_in_ms();
   table->philos[i].last_meal = 0;
   table->philos[i].times_ate = 0;
   table->philos[i].died = 0;
   table->philos[i].time_to_die = to_int(argv[2]);
   table->philos[i].time_to_eat = to_int(argv[3]);
   table->philos[i].time_to_sleep = to_int(argv[4]);
   table->philos[i].print_lock = &table->print_lock;
   table->philos[i].num_philos = table->num_philos;
}

void init_table(t_table *table,int argc, char *argv[])
{
   int i;

   table->num_philos = to_int(argv[1]);
   table->must_eat_count = -1;
   if(argc == 6)
      table->must_eat_count = to_int(argv[5]);
   i = 0;
   while(i < table->num_philos)
      pthread_mutex_init(&table->forks[i++], NULL);
   i = 0;
   while(i < table->num_philos)
      init_philos(table, i++,argv);
}

void *one_philo(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    pthread_mutex_lock(philo->lfork);
    philo_print(philo->print_lock, philo, "has taken a fork");
    ft_usleep(philo->time_to_die);
    pthread_mutex_unlock(philo->lfork);
    philo_print(philo->print_lock, philo, "died");
    return NULL;
}

void *death_check(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    int j;
    long timestamp;

    while(table->philos[0].died == 0)
    {
        i = 0;
        timestamp = get_time_in_ms() - table->philos[i].start_time;
        while(i < table->num_philos)
        {
            if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
            {
                j = 0;
                pthread_mutex_lock(&table->print_lock);
                printf("%ld %d died\n",timestamp, i + 1);
                while(j < table->num_philos)
                    table->philos[j++].died = 1;
                pthread_mutex_unlock(&table->print_lock);
                return NULL;
            }
            i++;
        }
    }
    return NULL;
}

void *eat_check(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;

    while(table->philos[0].died == 0)
    {
        i = 0;
        while(i < table->num_philos)
        {
            if(table->philos[i].times_ate < table->must_eat_count)
                break;
            i++;
        }
        if(i == table->num_philos && table->philos[0].died == 0)
        {
            pthread_mutex_lock(&table->print_lock);
            i = 0;
            while(i < table->num_philos)
                    table->philos[i++].died = 1;
            pthread_mutex_unlock(&table->print_lock);
            return NULL;
        }
    }
    return NULL;
}

void start_philo(t_table *table)
{
   int i;

   i = 1;
   while(i < table->num_philos)
   {
      pthread_create(&table->philos[i].thread, NULL, philosopher, &table->philos[i]);
      i+= 2;
   }
   i = 0;
   while(i < table->num_philos - 1)
   {
      pthread_create(&table->philos[i].thread, NULL, philosopher, &table->philos[i]);
      i += 2;
   }
   if(table->num_philos % 2 == 1)
      pthread_create(&table->philos[table->num_philos - 1].thread, NULL, philosopher, &table->philos[table->num_philos - 1]);
   pthread_create(&table->checker, NULL, death_check, &table);
   if(table->must_eat_count != -1)
      pthread_create(&table->checker, NULL, eat_check, &table);
   pthread_join(table->checker, NULL);
}

int main(int argc, char *argv[])
{
   int i;
   t_table *table;
   
   if((argc != 5 && argc != 6) || invalid_args(argc, argv))
      return(printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat].\n"),0);
   table = (t_table *)malloc(sizeof(t_table));
   if(!table)
      return (printf("Error: Failed to allocate memory for table.\n"), 0);
   init_table(table,argc, argv);
   if (!table)
      return (printf("Error: Failed to initialize table.\n"), 0);
   if(table->num_philos == 1)
      pthread_create(&table->philos[0].thread, NULL, one_philo, &table->philos[0]);
   else
      start_philo(table);
   i = 0;
   while(i < table->num_philos)
      pthread_join(table->philos[i++].thread, NULL);
   i = 0;
   while(i < table->num_philos)
      pthread_mutex_destroy(&table->forks[i++]);
   pthread_mutex_destroy(&table->print_lock);
   free(table);
   return 0;
}
