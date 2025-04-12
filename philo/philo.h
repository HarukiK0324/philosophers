/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkasamat <hkasamat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:50:32 by hkasamat          #+#    #+#             */
/*   Updated: 2025/04/12 20:50:33 by hkasamat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# define MAX_PHILOS 200

typedef struct s_philo
{
	int				id;
	int				num_philos;
	pthread_t		thread;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	*print_lock;
	long			start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			last_meal;
	int				eat_count;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				num_philos;
	int				must_eat_count;
	int				died;
	int				all_ate;
	pthread_t		eat_thread;
	pthread_t		death_thread;
	t_philo			philos[MAX_PHILOS];
	pthread_mutex_t	forks[MAX_PHILOS];
	pthread_mutex_t	print_lock;
	pthread_mutex_t	eat_count_lock;
	pthread_mutex_t	last_meal_lock;
}					t_table;

void				init_philos(t_table *table, int i, char *argv[]);
void				init_table(t_table *table, int argc, char *argv[]);
void				*one_philo(void *arg);
void				create_philo(t_table *table);
void				get_fork(t_philo *philo);
void				*philosopher(void *arg);
int					check_time(t_table *table, int i);
void				*death_check(void *arg);
void				check_eat_count(t_table *table, int *all_ate);
void				*eat_check(void *arg);
long				get_time(void);
int					ft_usleep(long time);
void				philo_print(pthread_mutex_t *print_lock, t_philo *philo,
						char *message);
int					str_len(char *str);
int					to_int(char *str);
int					check_num(char *str);
int					max(int a, int b);
int					invalid_args(int argc, char *argv[]);

#endif