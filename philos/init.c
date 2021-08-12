#include "philos.h"

static void	set_values(t_args *args, char **argv)
{
	args->n_times_to_eat = -1;
	args->philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		args->n_times_to_eat = ft_atoi(argv[5]);
	args->start_threads = 0;
	args->curr_time = 0;
	args->start_time = 0;
	args->dead = 0;
	args->stopped_philos = 0;
	args->unclock_write = 0;
}

t_args	*init_args(char **argv)
{
	t_args	*args;

	args = malloc(sizeof(t_args));
	if (!args)
	{	
		print_error_message(MALLOC);
		return (0);
	}
	set_values(args, argv);
	if (pthread_mutex_init(&args->write, 0) != 0)
	{
		print_error_message(MUTEX);
		return (0);
	}
	if (pthread_mutex_init(&args->start, 0) != 0)
	{
		print_error_message(MUTEX);
		return (0);
	}
	return (args);
}

int	init_forks(t_fork **forks, t_args *args)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (i < args->philos && !ret)
	{
		forks[i] = malloc(sizeof(t_fork));
		if (!forks[i])
			return (print_error_message(MALLOC));
		ret = fill_fork(forks[i], i);
		i++;
	}
	return (ret);
}

t_philo	**alloc_philos(t_args *args, int *ret)
{
	t_philo	**philos;

	philos = malloc(sizeof(t_philo *) * (args->philos + 1));
	if (!philos)
		*ret = print_error_message(MALLOC);
	philos[args->philos] = 0;
	return (philos);
}

t_philo	**init_philos(t_args *args)
{
	t_philo	**philos;
	t_fork	**forks;
	int		ret;
	int		i;

	ret = 0;
	forks = malloc(sizeof(t_fork *) * (args->philos + 1));
	if (!forks)
		ret = print_error_message(MALLOC);
	forks[args->philos] = 0;
	philos = alloc_philos(args, &ret);
	i = -1;
	while (++i < args->philos && !ret)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			ret = print_error_message(MALLOC);
		if (!ret)
			ret = fill_philo(philos[i], forks, args, i);
	}
	if (!ret)
		ret = init_forks(forks, args);
	if (ret)
		return (0);
	return (philos);
}
