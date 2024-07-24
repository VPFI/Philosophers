/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:46 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/24 16:58:11 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long long	get_time(void)
{
	static struct timeval	tv;
	long long				res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}

void* routine(void *args)
{
	t_philo *philo;

	philo = args;
	printf("Holaaaaa I'm: %i\n", philo->id);
	return (NULL);
}

int	create_philos(t_global *global)
{
	int	i;

	i = 0;
	global->philo = (t_philo *)calloc(global->philo_amount, sizeof(t_philo));
	while(i < global->philo_amount)
	{
		global->philo[i].global = global;
		global->philo[i].id = i;
		global->philo[i].left_fork = &global->forks[i];
		if (i == 0)
			global->philo[i].right_fork = &global->forks[global->philo_amount - 1];
		else
			global->philo[i].right_fork = &global->forks[i - 1];
		if (pthread_create(&(global->philo[i].self), NULL, &routine, (void *)&global->philo[i]))
			return (1);
		//sleep(1);
		i++;
	}
	return (0);
}

int	create_forks(t_global *global)
{
	int	i;

	i = 0;
	global->forks = (pthread_mutex_t *)calloc((global->philo_amount - 1), sizeof(pthread_mutex_t));
	while (i < global->philo_amount)
	{
		if (pthread_mutex_init(&global->forks[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

void	init_global(t_global *g, char **argv)
{
	g->philo_amount = atoi(argv[1]);
	g->time_die = atoi(argv[2]);
	g->time_eat = atoi(argv[3]);
	g->time_sleep = atoi(argv[4]);
	g->eating_needs = atoi(argv[5]);
	g->death_flag = 0;
}

int	main(int argc, char **argv)
{
	t_global	global;
	int			i;

	i = 0;
	if (argc != 6)
		return (1);
	//check number of philos...
	init_global(&global, argv);
	if (create_forks(&global))
		return (1);
	if (create_philos(&global))
		return (1);
	while (i < global.philo_amount)
	{
		if (pthread_join(global.philo[i].self, NULL))
			return (1);
		printf("joined %i\n", global.philo[i].id);
		i++;
	}
	free(global.philo);
	free(global.forks);
	return (0);
}