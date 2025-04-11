#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#define MAX_PHILOS 200

typedef struct s_philo
{
    int id;
    int num_philos;
    pthread_t thread;
    pthread_mutex_t *lfork;
    pthread_mutex_t *rfork;
    pthread_mutex_t *print_lock;
    long start_time;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    long last_meal;
    int times_ate;
    int died;
} t_philo;

typedef struct s_table
{
    int num_philos;
    int must_eat_count;
    int died;
    int all_ate;
    pthread_t eat_thread;
    pthread_t death_thread;
    t_philo philos[MAX_PHILOS];
    pthread_mutex_t forks[MAX_PHILOS];
    pthread_mutex_t print_lock;
} t_table;

int str_len(char *str);
int	to_int(char *str);
int check_num(char *str);


#endif