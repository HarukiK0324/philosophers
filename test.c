#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
    int must_eat_count;
    int died;
} t_philo;

typedef struct s_table
{
    int num_philos;
    pthread_t checker;
    t_philo philos[200];
    pthread_mutex_t forks[200];
    pthread_mutex_t print_lock;
} t_table;

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
    while (get_time_in_ms() - start < time)
        usleep(100);
    return 0;
}

void philo_print(pthread_mutex_t *print_lock, t_philo *philo, const char *message)
{
    long timestamp;
    
    timestamp = get_time_in_ms() - philo->start_time;
    pthread_mutex_lock(print_lock);
    if(philo->died ==0)
        printf("%ld %d %s\n", timestamp, philo->id + 1, message);
    pthread_mutex_unlock(print_lock);
}

int philo_died()
{
    return 0;
}

void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;

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
        philo->times_ate++;
        ft_usleep(philo->time_to_eat);
        philo->last_meal = get_time_in_ms() - philo->start_time;
        pthread_mutex_unlock(philo->lfork);
        pthread_mutex_unlock(philo->rfork);
        philo_print(philo->print_lock, philo, "is sleeping");
        ft_usleep(philo->time_to_sleep);
        philo_print(philo->print_lock, philo, "is thinking");
        if(philo->num_philos%2 == 1)
            ft_usleep(philo->time_to_eat);
    }
    return NULL;
}

void init_philos(t_table *table, int i, long start_time)
{
    table->philos[i].id = i;
    table->philos[i].lfork = &table->forks[i];
    table->philos[i].rfork = &table->forks[(i + 1) % table->num_philos];
    table->philos[i].start_time = start_time;
    table->philos[i].time_to_die = 810;
    table->philos[i].time_to_eat = 400;
    table->philos[i].time_to_sleep = 400;
    table->philos[i].last_meal = start_time;
    table->philos[i].times_ate = 0;
    table->philos[i].must_eat_count = 0;
    table->philos[i].died = 0;
    table->philos[i].print_lock = &table->print_lock;
    table->philos[i].num_philos = table->num_philos;
}

void *death_check(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    int j;
    long timestamp;

    while(1)
    {
        i = 0;
        while(i < table->num_philos)
        {
            timestamp = get_time_in_ms() - table->philos[i].start_time;
            if (timestamp - table->philos[i].last_meal > table->philos[i].time_to_die)
            {
                j = 0;
                pthread_mutex_lock(&table->print_lock);
                printf("%ld %d died\n",timestamp, i + 1);
                while(j < table->num_philos)
                    table->philos[j++].died = 1;
                return NULL;
            }
            i++;
        }
    }
}

int main()
{
    long start_time;
    t_table table;
    table.num_philos = 6;
    int i;

    for (i = 0; i < table.num_philos; i++)
        pthread_mutex_init(&table.forks[i], NULL);
    pthread_mutex_init(&table.print_lock, NULL);
    start_time = get_time_in_ms();
    for (i = 0; i < table.num_philos; i++)
        init_philos(&table, i, start_time);
    for (i = 0; i < table.num_philos - 1; i+= 2)
        pthread_create(&table.philos[i].thread, NULL, philosopher, &table.philos[i]);
    for(i = 1;i < table.num_philos; i+= 2)
        pthread_create(&table.philos[i].thread, NULL, philosopher, &table.philos[i]);
    if(table.num_philos % 2 == 1)
        pthread_create(&table.philos[table.num_philos - 1].thread, NULL, philosopher, &table.philos[table.num_philos - 1]);
    pthread_create(&table.checker, NULL, death_check, &table);
    for (i = 0; i < table.num_philos; i++)
        pthread_join(table.philos[i].thread, NULL);
    pthread_join(table.checker, NULL);
    pthread_mutex_destroy(&table.print_lock);
    for (i = 0; i < table.num_philos; i++)
        pthread_mutex_destroy(&table.forks[i]);
    return 0;
}
