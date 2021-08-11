#include "philos.h"

int	print_error_message(int error_type)
{
	if (error_type == MALLOC)
		printf("Error: Malloc error occured\n");
	else if (error_type == INV_ARG)
		printf("Error: Invalid arguments\n");
	else if (error_type == LONELY_PHILO)
		printf("Error: No philosopher can eat with just one fork\n");
	else if (error_type == GET_TIME)
		printf("Error: \"Get time\" error occured\n");
	return (1);
}

// void	print_routine_message(int num, )

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	if (argc != 5 && argc != 6)
		return (print_error_message(INV_ARG));
	while (argv[++i])
	{
		j = -1;
		while(argv[i][++j])
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_error_message(INV_ARG));
	}
	if (argv[1][0] == '1' && !argv[1][1])
		return (print_error_message(LONELY_PHILO));
	return (0);
}

int	ft_atoi(char *str)
{
	int	value;
	
	value = 0;
	while (*str && *str >= '0' && *str <= '9')
		value = 10 * value + (*str++ - '0');
	return (value);
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
	args->n_times_to_eat = -1;
	args->philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		args->n_times_to_eat = ft_atoi(argv[5]);
	args->start_threads = 0;
	args->curr_time = 0;
	args->dead = 0;
	args->stopped_philos = 0;
	args->write = malloc(sizeof(pthread_mutex_t));
	if (!args->write)
	{
		print_error_message(MALLOC);
		return (0);
	}
	return (args);
}

int	fill_fork(t_fork *fork, int num)
{
	num++;
	fork->num = num;
	fork->mutex = malloc(sizeof(pthread_mutex_t));
	if (!fork->mutex)
		return (print_error_message(MALLOC));
	if (pthread_mutex_init(fork->mutex, 0) != 0)
		return (print_error_message(MUTEX));
	return (0);
}

int	init_forks(t_fork **forks, t_args *args)
{
	int	i;
	int		ret;

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

int	fill_philo(t_philo *philo, t_fork **forks, t_args *args, int num)
{
	num++;
	philo->num = num;
	philo->l_fork = num;
	philo->r_fork = num + 1;
	if (num == args->philos)
		philo->r_fork = 1;
	philo->args = args;
	philo->forks = forks;
	philo->n_times_ate = 0;
	philo->last_meal = 0;
	philo->stopped = 0;
	return (0);
}

t_philo **init_philos(t_args *args)
{
	t_philo	**philos;
	t_fork	**forks;
	int		ret;
	int		i;

	ret = 0;
	philos = malloc(sizeof(t_philo*) * (args->philos + 1));
	if (!philos)
		ret = print_error_message(MALLOC);
	philos[args->philos] = 0;
	forks = malloc(sizeof(t_fork*) * (args->philos + 1));
	if (!forks)
		ret = print_error_message(MALLOC);
	forks[args->philos] = 0;
	i = 0;
	while (i < args->philos && !ret)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			ret = print_error_message(MALLOC);
		if (!ret)
			ret = fill_philo(philos[i], forks, args, i);
		i++;
	}
	ret = init_forks(forks, args);
	if (ret)
		return (0);
	return (philos);
}

void	ft_eat(t_philo *philo)
{
	struct timeval	start_time;

	pthread_mutex_lock(philo->forks[philo->l_fork - 1]->mutex);
	pthread_mutex_lock(philo->args->write);
	if (!philo->args->start_time)
	{
		gettimeofday(&start_time, 0);
		philo->args->start_time = (start_time.tv_sec * 1000) + (start_time.tv_usec / 1000);
	}
	gettimeofday(&philo->curr_time, 0);
	philo->args->curr_time = (philo->curr_time.tv_sec * 1000) + (philo->curr_time.tv_usec / 1000)
							- philo->args->start_time;
	printf("%dms %d has taken a fork\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(philo->args->write);
	//another fork
	pthread_mutex_lock(philo->forks[philo->r_fork - 1]->mutex);
	pthread_mutex_lock(philo->args->write);
	gettimeofday(&philo->curr_time, 0);
	philo->args->curr_time = (philo->curr_time.tv_sec * 1000) + (philo->curr_time.tv_usec / 1000)
								- philo->args->start_time;
	printf("%dms %d has taken a fork\n", philo->args->curr_time, philo->num);
	gettimeofday(&philo->curr_time, 0);
	philo->args->curr_time = (philo->curr_time.tv_sec * 1000) + (philo->curr_time.tv_usec / 1000)
								- philo->args->start_time;
	philo->last_meal = philo->args->curr_time;
	printf("%dms %d is eating\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(philo->args->write);
	usleep(philo->args->time_to_eat * 1000);
	pthread_mutex_unlock(philo->forks[philo->l_fork - 1]->mutex);
	pthread_mutex_unlock(philo->forks[philo->r_fork - 1]->mutex);
}

void	ft_sleep_n_think(t_philo *philo)
{
	pthread_mutex_lock(philo->args->write);
	gettimeofday(&philo->curr_time, 0);
	philo->args->curr_time = (philo->curr_time.tv_sec * 1000) + (philo->curr_time.tv_usec / 1000)
								- philo->args->start_time;
	printf("%dms %d is sleeping\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(philo->args->write);
	usleep(philo->args->time_to_sleep * 1000);
	pthread_mutex_lock(philo->args->write);
	gettimeofday(&philo->curr_time, 0);
	philo->args->curr_time = (philo->curr_time.tv_sec * 1000) + (philo->curr_time.tv_usec / 1000)
								- philo->args->start_time;
	printf("%dms %d is thinking\n", philo->args->curr_time, philo->num);
	pthread_mutex_unlock(philo->args->write);
}

void	*philo_routine(void *philo)
{
	t_philo 		*work;
		
	work = (t_philo *) philo;
	while (!work->args->start_threads)
		;
	if (!(work->num % 2))
		usleep(5000);
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
		// work->dead++;
	}
	return (0);
}

void	ft_death_control(t_philo **philos, t_args *args)
{
	int		i;
	int		curr_time;
	struct	timeval time;

	while (!args->dead && args->stopped_philos != args->philos)
	{
		i = -1;
		while (philos[++i] && args->start_time)
		{
			if (!philos[i]->stopped)
			{
				pthread_mutex_lock(args->write);
				gettimeofday(&time, 0);
				curr_time = (time.tv_sec * 1000) + (time.tv_usec / 1000) - args->start_time;
				if (curr_time - philos[i]->last_meal > args->time_to_die)
				{
					printf("%dms %d died\n", curr_time, philos[i]->num);
					args->dead = 1;
					i = -1;
					while (philos[++i])
						pthread_detach(philos[i]->thread);
				}
				pthread_mutex_unlock(args->write);
			}
		}
	}
}

int	start_philos(t_philo **philos, t_args *args)
{
	int	i;

	i = -1;
	if (args->n_times_to_eat == 0)
		return (0);
	while (++i < args->philos)
	{
		pthread_create(&philos[i]->thread, 0, &philo_routine, (void *) philos[i]);
		usleep(5000);
	}
	args->start_threads = 1;
	ft_death_control(philos, args);
	i = -1;
	while (++i < args->philos)
		pthread_join(philos[i]->thread, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_args	*args;
	t_philo **philos;

	if (!check_args(argc, argv))
	{
		args = init_args(argv);
		if (!args)
			return (1);
		philos = init_philos(args);
		if (!philos)
			return (1);
		if (start_philos(philos, args))
			return (1);
	}
	else
		return (1);
	// printf("Philos number %lld \n", (unsigned long long)args->philos);
	// printf("Time_to_die %lld \n", (unsigned long long)args->time_to_die);
	// printf("Time_to_eat %lld \n", (unsigned long long)args->time_to_eat);
	// printf("Time_to_sleep %lld \n", (unsigned long long)args->time_to_sleep);
	// if (args->n_times_to_eat)
	// 	printf("Number of times to eat %lld \n", (unsigned long long)args->n_times_to_eat);

	return (0);
}