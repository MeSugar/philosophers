#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t mutex;
int j = 0;

void	*foo(void *primes)
{
	int	i;
	int	*sum;
	int *arr;

	arr = (int*)primes;
	sum = malloc(sizeof(int));
	*sum = arr[j];
	i = 0;
	while (i < 4)
	{
		i++;
		pthread_mutex_lock(&mutex);
		*sum = *sum + arr[j + 1];
		j++;
		pthread_mutex_unlock(&mutex);
	}
	j++;
	printf("first value is %d\n", *sum);
	// printf("Thread res is %d\n", sum);
	return ((void*) sum);
}

int main()
{
	pthread_t	th[2];
	int i;
	int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
	int *sum;
	int total;

	i = 0;
	// srand(time(0));
	pthread_mutex_init(&mutex, 0);
	while (i < 2)
	{
		pthread_create(&th[i], 0, &foo, (void *) primes);
		// printf("hello from thread %d\n", i);
		i++;
	}
	i = 0;
	total = 0;
	while (i < 2)
	{
		pthread_join(th[i], (void *) &sum);
		// printf("value of %d tread is %d\n", i, *value);
		printf("Thread %d res is %d\n", i, *sum);
		total += *sum;
		free(sum);
		i++;
	}
	printf("Total is %d\n", total);
	pthread_mutex_destroy(&mutex);
	return (0);
}