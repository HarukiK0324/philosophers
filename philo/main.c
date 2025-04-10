/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:29:00 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/10 09:06:20 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philosopher(int *fork[])
{
    printf("Philosopher thread started\n");
    while(!philo_died())
    {
      pthread_mutex_lock(&fork[0]);
      printf("Philosopher picked up fork 1\n");
      pthread_mutex_lock(&fork[1]);
      printf("Philosopher picked up fork 2\n");
      printf("Philosopher is eating\n");
      ft_usleep(200);
      pthread_mutex_unlock(&fork[0]);
      pthread_mutex_unlock(&fork[1]);
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
   return (0);
}

pthread_mutex_t *init_fork()
{
   pthread_mutex_t fork;

   if (pthread_mutex_init(&fork, NULL) != 0)
      return (NULL);
   return (&fork);
}

void init_philos(t_table *table, int i)
{
   table->philos[i].id = i;
   table->philos[i].lfork = &table->forks[i];
   table->philos[i].rfork = &table->forks[(i + 1) % table->num_philos];
   table->philos[i].last_meal = 0;
   table->philos[i].times_ate = 0;
   table->philos[i].died = 0;
}

t_table *init_table(int argc, char *argv[])
{
   t_table *table;
   int i;

   table = (t_table *)malloc(sizeof(t_table));
   table->num_philos = to_int(argv[1]);
   table->time_to_die = to_int(argv[2]);
   table->time_to_eat = to_int(argv[3]);
   table->time_to_sleep = to_int(argv[4]);
   table->must_eat_count = -1;
   if(argc == 6)
      table->must_eat_count = to_int(argv[5]);
   i = -1;
   while(i++ < table->num_philos)
      &table->forks[i] = init_fork();
   i = -1;
   while(i++ < table->num_philos)
      init_philos(table, i);
}

int main(int argc, char *argv[])
{
   t_table *table;
   struct timeval time;
   long milisec;
   
   gettimeofday(&time, NULL);
   if((argc != 5 && argc != 6) || invalid_args(argc, argv))
      return(printf("Format: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat].\n"),0);
   table = init_table(argc, argv);
   if (!table)
      return (printf("Error: Failed to initialize table.\n"), 0);
   
}
