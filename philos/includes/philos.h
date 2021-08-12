/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdelta <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 16:12:52 by gdelta            #+#    #+#             */
/*   Updated: 2021/08/12 16:12:55 by gdelta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_args
{
	int				philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_times_to_eat;
	int				start_threads;
	int				dead;
	int				stopped_philos;
	int				unclock_write;
	long int		start_time;
	long int		curr_time;
	pthread_mutex_t	write;
	pthread_mutex_t	start;
}			t_args;

typedef struct s_fork
{
	int				num;
	pthread_mutex_t	*mutex;

}				t_fork;

typedef struct s_philo
{
	int				num;
	int				l_fork;
	int				r_fork;
	int				n_times_ate;
	int				last_meal;
	int				stopped;
	pthread_t		thread;
	t_fork			**forks;
	t_args			*args;
}				t_philo;

t_args		*init_args(char **argv);
int			init_forks(t_fork **forks, t_args *args);
t_philo		**init_philos(t_args *args);
int			print_error_message(int error_type);
long int	get_curr_time(void);
void		ft_usleep(int time_to_sleep);
int			ft_atoi(char *str);
int			check_args(int argc, char **argv);
int			fill_fork(t_fork *fork, int num);
int			fill_philo(t_philo *philo, t_fork **forks, t_args *args, int num);
void		*philo_routine(void *philo);
void		ft_death_control(t_philo **philos, t_args *args);

#endif
