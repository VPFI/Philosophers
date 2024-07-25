/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:46 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/25 20:44:56 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	writer(int mode, int id, t_global *global)
{
	pthread_mutex_lock(&global->printer);
	if (mode == FORK)
		printf("%lli %i has taken a fork\n", diff_time(global->start_time), id);
	if (mode == EAT)
		printf("%lli %i is eating\n", diff_time(global->start_time), id);
	if (mode == SLEEP)
		printf("%lli %i is sleeping\n", diff_time(global->start_time), id);
	if (mode == THINK)
		printf("%lli %i is thinking\n", diff_time(global->start_time), id);
	if (mode == DEATH)
		printf("%lli %i died\n", diff_time(global->start_time), id);
	pthread_mutex_unlock(&global->printer);
}


long long	get_time(void)
{
	static struct timeval	tv;
	long long				res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}
void	ft_sleep(int m_sec, int *flag)
{
	long long	i;

	i = get_time() + m_sec;
	//mutex
	while (!(*flag) && get_time() < i)
	{
		usleep(500);
	}
	return ;
}

long long	diff_time(long long start_time)
{
	long long res;

	res = -1;
	res = get_time() - start_time;
	return (res);
}

void	philo_sleep(t_philo *philo)
{
	//mutex
	writer(SLEEP, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, &philo->global->stop_flag);
}

void	eat(t_philo *philo)
{
	//mutex
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_lock(philo->right_fork);
	writer(FORK, philo->id, philo->global);
	writer(EAT, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, &philo->global->stop_flag);
	//mutex
	philo->last_eat = get_time();
	//mutex
	philo->times_eat += 1;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void*	routine(void *args)
{
	t_philo *philo;

	philo = args;
	philo->last_eat = get_time();
	//mutex
	while (!(philo->global->stop_flag))
	{
		eat(philo);
		if (!(philo->global->stop_flag))			
			philo_sleep(philo);
	}
	return (NULL);
}

int	create_philos(t_global *global)
{
	int	i;

	i = 0;
	global->philo = (t_philo *)calloc(global->philo_amount, sizeof(t_philo));
	global->start_time = get_time();
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
		ft_sleep(10, &global->stop_flag);
		i++;
	}
	return (0);
}

int	create_forks(t_global *global)
{
	int	i;

	i = 0;
	global->forks = (pthread_mutex_t *)calloc((global->philo_amount), sizeof(pthread_mutex_t));
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
	g->stop_flag = 0;
	pthread_mutex_init(&g->printer, NULL);
}

int	supervise(t_global *global)
{
	int	i;
	int	eat_flag;

	i = 0;
	eat_flag = 1;
	while (i < global->philo_amount)
	{
		//mutex
		if ((get_time() - global->philo[i].last_eat) > global->time_die)
		{
			global->stop_flag = 1;
			printf("DEATH by inanition\n");
			return (1);
		}
		if (global->philo[i].times_eat < global->eating_needs)
			eat_flag = 0;
		i++;
	}
	if (eat_flag)
	{
		global->stop_flag = 1;
		printf("Gluttony\n");
		return (1);
	}
	return (0);
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
	while (!global.stop_flag)
		if (supervise(&global))
			break;
	printf("TERMINATED ++++++++++++++\n");
	while (i < global.philo_amount)
	{
		if (pthread_join(global.philo[i].self, NULL))
			return (1);
		//printf("joined %i\n", global.philo[i].id);
		i++;
	}
	free(global.philo);
	free(global.forks);
	return (0);
}