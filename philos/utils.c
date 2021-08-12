#include "philos.h"

int	print_error_message(int error_type)
{
	if (error_type == MALLOC)
		printf("Error: Malloc error occured\n");
	else if (error_type == INV_ARG)
		printf("Error: Invalid arguments\n");
	else if (error_type == GET_TIME)
		printf("Error: \"Get time\" error occured\n");
	return (1);
}

int	ft_atoi(char *str)
{
	int	value;

	value = 0;
	while (*str && *str >= '0' && *str <= '9')
		value = 10 * value + (*str++ - '0');
	return (value);
}

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
		while (argv[i][++j])
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_error_message(INV_ARG));
	}
	return (0);
}

long int	get_curr_time(void)
{
	struct timeval	curr_time;
	long int		ret;

	gettimeofday(&curr_time, 0);
	ret = (curr_time.tv_sec * 1000) + (curr_time.tv_usec / 1000);
	return (ret);
}

void	ft_usleep(int time_to_sleep)
{
	unsigned long	last_time;

	last_time = get_curr_time() + time_to_sleep;
	while (1)
	{
		if (get_curr_time() >= last_time)
			break ;
		usleep(500);
	}
}
