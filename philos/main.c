/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdelta <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 16:12:21 by gdelta            #+#    #+#             */
/*   Updated: 2021/08/12 16:12:26 by gdelta           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

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

int	start_philos(t_philo **philos, t_args *args)
{
	int	i;

	i = -1;
	if (args->n_times_to_eat == 0)
		return (0);
	pthread_mutex_lock(&args->start);
	while (++i < args->philos)
	{
		pthread_create(&philos[i]->thread, 0,
			&philo_routine, (void *) philos[i]);
		usleep(100);
	}
	pthread_mutex_unlock(&args->start);
	ft_death_control(philos, args);
	i = -1;
	while (++i < args->philos)
		pthread_join(philos[i]->thread, 0);
	return (0);
}

void	mutex_destroyer(t_args *args, t_philo **philos)
{
	int	i;

	pthread_mutex_destroy(&args->start);
	pthread_mutex_destroy(&args->write);
	i = -1;
	while (++i < args->philos)
		pthread_mutex_destroy(philos[0]->forks[i]->mutex);
}

int	main(int argc, char **argv)
{
	t_args	*args;
	t_philo	**philos;

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
		if (args->unclock_write)
			pthread_mutex_unlock(&args->write);
		mutex_destroyer(args, philos);
	}
	else
		return (1);
	return (0);
}
