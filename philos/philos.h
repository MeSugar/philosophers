#ifndef PHILOS_H
# define PHILOS_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define MALLOC 20
# define INV_ARG 21
# define GET_TIME 22
# define MUTEX 23


typedef struct	s_args
{
	int				philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_times_to_eat;
	int				start_threads;
	int				start_time;
	int				curr_time;
	int				dead;
	int				stopped_philos;
	int				unclock_write;
	pthread_mutex_t	*write;
}				t_args;

typedef struct	s_fork
{
	int				num;
	pthread_mutex_t	*mutex;

}				t_fork;

typedef struct	s_philo
{
	int				num;
	int				l_fork;
	int				r_fork;
	int				n_times_ate;
	int				last_meal;
	int				stopped;
	pthread_t		thread;
	struct timeval	curr_time;
	t_fork			**forks;
	t_args			*args;
}				t_philo;

#endif
