/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/11 15:46:26 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int str_len(char *str)
{
    int	i;

    i = 0;
    if(!str)
        return 0;
    while (str[i] != '\0')
        i++;
    return (i);
}

int	to_int(char *str)
{
	int	i;
	int	itr;
	long num;

    if(str_len(str) > 10)
        return (-1);
	num = 0;
	i = 0;
	itr = 1;
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
    if(num > INT32_MAX)
        return (-1);
	return (num);
}

int check_num(char *str)
{
    int i;

    i = 0;
    if(!str)
        return (-1);
    while (str[i] != '\0')
    {
        if(str[i] < '0' || str[i] > '9')
            return (-1);
        i++;
    }
    return (0);
}

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
    if (philo->died)
    {
        pthread_mutex_unlock(print_lock);
        return;
    }
    timestamp = get_time() - philo->start_time;
    printf("%ld %d %s\n", timestamp, philo->id + 1, message);
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
        philo->last_meal = get_time();
        philo_print(philo->print_lock, philo, "is eating");
        ft_usleep(philo->time_to_eat);
        philo->times_ate++;
        philo_print(philo->print_lock, philo, "is sleeping");
        philo->last_meal = get_time();
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
   if(to_int(argv[1]) > MAX_PHILOS)
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
   table->philos[i].start_time = get_time();
   table->philos[i].last_meal = table->philos[i].start_time;
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

   table->died = 0;
   table->num_philos = to_int(argv[1]);
   table->must_eat_count = -1;
   if(argc == 6)
      table->must_eat_count = to_int(argv[5]);
   i = 0;
   while(i < table->num_philos)
      pthread_mutex_init(&table->forks[i++], NULL);
    pthread_mutex_init(&table->print_lock, NULL);
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
    return NULL;
}

void *death_check(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    int j;
    long timestamp;
    
    while(table->all_ate == 0)
    {
        i = -1;
        while(++i < table->num_philos)
        {
            timestamp = get_time();
            if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
            {
                table->died = 1;
                j = 0;
                pthread_mutex_lock(&table->print_lock);
                printf("%ld %d died\n",get_time() - table->philos[i].start_time, i + 1);
                while(j < table->num_philos)
                    table->philos[j++].died = 1;
                pthread_mutex_unlock(&table->print_lock);
                return NULL;
            }
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
        if(i == table->num_philos && table->died == 0)
        {
            table->all_ate = 1;
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

void create_philo(t_table *table)
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
}

int main(int argc, char *argv[])
{
   int i;
   t_table table;
   
   if((argc != 5 && argc != 6) || invalid_args(argc, argv))
      return(printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"),0);
   init_table(&table,argc, argv);
   if(table.num_philos == 1)
      pthread_create(&table.philos[0].thread, NULL, one_philo, &table.philos[0]);
   else
      create_philo(&table);
    pthread_create(&table.death_thread, NULL, death_check, &table);
    if(table.must_eat_count != -1)
        pthread_create(&table.eat_thread, NULL, eat_check, &table);
   i = 0;
   while(i < table.num_philos)
      pthread_join(table.philos[i++].thread, NULL);
    pthread_join(table.death_thread, NULL);
    if (table.must_eat_count != -1)
        pthread_join(table.eat_thread, NULL);
   i = 0;
   while(i < table.num_philos)
      pthread_mutex_destroy(&table.forks[i++]);
   pthread_mutex_destroy(&table.print_lock);
   return 0;
}
