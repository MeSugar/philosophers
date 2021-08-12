#include "philos.h"

static void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->forks[philo->l_fork - 1]->mutex);
	pthread_mutex_lock(&philo->args->write);
	philo->args->curr_time = get_curr_time() - philo->args->start_time;
	if (!philo->args->dead)
		printf("%ld %d has taken a fork\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(&philo->args->write);
	pthread_mutex_lock(philo->forks[philo->r_fork - 1]->mutex);
	pthread_mutex_lock(&philo->args->write);
	philo->args->curr_time = get_curr_time() - philo->args->start_time;
	if (!philo->args->dead)
		printf("%ld %d has taken a fork\n", philo->args->curr_time, philo->num);
	philo->last_meal = philo->args->curr_time;
	if (!philo->args->dead)
		printf("%ld %d is eating\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(&philo->args->write);
	ft_usleep(philo->args->time_to_eat);
	pthread_mutex_unlock(philo->forks[philo->l_fork - 1]->mutex);
	pthread_mutex_unlock(philo->forks[philo->r_fork - 1]->mutex);
}

static void	ft_sleep_n_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->write);
	philo->args->curr_time = get_curr_time() - philo->args->start_time;
	if (!philo->args->dead)
		printf("%ld %d is sleeping\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(&philo->args->write);
	ft_usleep(philo->args->time_to_sleep);
	pthread_mutex_lock(&philo->args->write);
	philo->args->curr_time = get_curr_time() - philo->args->start_time;
	if (!philo->args->dead)
		printf("%ld %d is thinking\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(&philo->args->write);
}

void	*philo_routine(void *philo)
{
	t_philo	*work;

	work = (t_philo *) philo;
	pthread_mutex_lock(&work->args->start);
	if (!work->args->start_time)
		work->args->start_time = get_curr_time();
	pthread_mutex_unlock(&work->args->start);
	if (!(work->num % 2))
		ft_usleep(5);
	while (!work->args->dead)
	{
		ft_eat(work);
		ft_sleep_n_think(work);
		if (work->args->n_times_to_eat != -1)
			work->n_times_ate++;
		if (work->n_times_ate == work->args->n_times_to_eat)
		{
			work->stopped = 1;
			work->args->stopped_philos++;
			return (0);
		}
	}
	return (0);
}

static void	death_handler(t_args *args, t_philo **philos,
							int i, long int curr_time)
{
	args->dead = 1;
	printf("%ld %d died\n", curr_time, philos[i]->num);
	i = -1;
	while (philos[++i])
		pthread_detach(philos[i]->thread);
	args->unclock_write = 1;
}

void	ft_death_control(t_philo **philos, t_args *args)
{
	int				i;
	long int		curr_time;

	while (!args->dead && args->stopped_philos != args->philos)
	{
		i = -1;
		while (philos[++i] && args->start_time)
		{
			if (!philos[i]->stopped)
			{
				pthread_mutex_lock(&args->write);
				curr_time = get_curr_time() - args->start_time;
				if (curr_time - philos[i]->last_meal > args->time_to_die)
				{
					death_handler(args, philos, i, curr_time);
					return ;
				}
				pthread_mutex_unlock(&args->write);
			}
		}
		usleep(1000);
	}
}
