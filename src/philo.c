/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:46 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/30 15:22:19 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	writer(int mode, int id, t_global *global)
{
	pthread_mutex_lock(&global->m_stop_flag);
	pthread_mutex_lock(&global->m_printer);
	if (global->stop_flag)
	{
		pthread_mutex_unlock(&global->m_stop_flag);
		pthread_mutex_unlock(&global->m_printer);
		return ;
	}
	if (mode != DEATH)
		pthread_mutex_unlock(&global->m_stop_flag);
	if (mode == FORK)
		printf("%lli %i has taken a fork\n", diff_time(global->start_time), id);
	else if (mode == EAT)
		printf("%lli %i is eating\n", diff_time(global->start_time), id);
	else if (mode == SLEEP)
		printf("%lli %i is sleeping\n", diff_time(global->start_time), id);
	else if (mode == THINK)
		printf("%lli %i is thinking\n", diff_time(global->start_time), id);
	else if (mode == DEATH)
	{
		global->stop_flag = 1;
		printf("%lli %i died\n", diff_time(global->start_time), id);
		pthread_mutex_unlock(&global->m_stop_flag);
	}
	pthread_mutex_unlock(&global->m_printer);
}


long long	get_time(void)
{
	static struct timeval	tv;
	long long				res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}
void	ft_sleep(int m_sec, t_global *global)
{
	long long	i;

	i = get_time() + m_sec;
	pthread_mutex_lock(&global->m_stop_flag);
	while (!global->stop_flag && get_time() < i)
	{
		pthread_mutex_unlock(&global->m_stop_flag);
		usleep(500);
		pthread_mutex_lock(&global->m_stop_flag);
	}
	pthread_mutex_unlock(&global->m_stop_flag);
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
	writer(SLEEP, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, philo->global);
}

void	eat_odd(t_philo *philo)
{
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_lock(philo->right_fork);
	writer(FORK, philo->id, philo->global);
	writer(EAT, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, philo->global);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	pthread_mutex_lock(&philo->m_times_eat);
	philo->times_eat += 1;
	pthread_mutex_unlock(&philo->m_times_eat);
}

void	eat_even(t_philo *philo)
{
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->right_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	writer(EAT, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, philo->global);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	pthread_mutex_lock(&philo->m_times_eat);
	philo->times_eat += 1;
	pthread_mutex_unlock(&philo->m_times_eat);
}

void*	routine(void *args)
{
	t_philo *philo;

	philo = args;
	pthread_mutex_lock(&philo->global->m_stop_flag);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	while (!(philo->global->stop_flag))
	{
		pthread_mutex_unlock(&philo->global->m_stop_flag);
		if (philo->id % 2)
			eat_odd(philo);
		else
			eat_even(philo);
		philo_sleep(philo);
		pthread_mutex_lock(&philo->global->m_stop_flag);
	}
	pthread_mutex_unlock(&philo->global->m_stop_flag);
	//printf("%i ENDED\n", philo->id);
	return (NULL);
}

int	create_philos(t_global *global)
{
	int	i;

	i = 0;
	global->philo = (t_philo *)calloc(global->philo_amount, sizeof(t_philo));
	if (!global->philo)
		return (1);
	pthread_mutex_lock(&global->m_stop_flag);
	while(!global->stop_flag && i < global->philo_amount)
	{
		//pthread_mutex_unlock(&global->m_stop_flag);
		global->philo[i].global = global;
		global->philo[i].id = i;
		global->philo[i].left_fork = &global->forks[i];
		pthread_mutex_init(&global->philo[i].m_last_eat, NULL);
		pthread_mutex_init(&global->philo[i].m_times_eat, NULL);
		if (i == 0)
			global->philo[i].right_fork = &global->forks[global->philo_amount - 1];
		else
			global->philo[i].right_fork = &global->forks[i - 1];
		if (pthread_create(&(global->philo[i].self), NULL, &routine, (void *)&global->philo[i]))
			return (1);
		//ft_sleep(10, global);
		i++;
		//pthread_mutex_lock(&global->m_stop_flag);
	}
	printf("Created philos\n");
	global->start_time = get_time();
	return (pthread_mutex_unlock(&global->m_stop_flag), 0);
}

int	create_forks(t_global *global)
{
	int	i;

	i = 0;
	global->forks = (pthread_mutex_t *)calloc((global->philo_amount), sizeof(pthread_mutex_t));
	if (!global->forks)
		return (1);
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
	pthread_mutex_init(&g->m_printer, NULL);
	pthread_mutex_init(&g->m_stop_flag, NULL);
}

int	supervise(t_global *global)
{
	int	i;
	int	eat_flag;

	i = 0;
	eat_flag = 1;
	while (i < global->philo_amount)
	{
		pthread_mutex_lock(&global->philo[i].m_last_eat);
		if (global->philo[i].last_eat > 0 && (get_time() - global->philo[i].last_eat) > global->time_die)
		{
			pthread_mutex_unlock(&global->philo[i].m_last_eat);
			writer(DEATH, global->philo[i].id, global);
			return (1);
		}
		pthread_mutex_unlock(&global->philo[i].m_last_eat);
		pthread_mutex_lock(&global->philo[i].m_times_eat);
		if (global->philo[i].times_eat < global->eating_needs)
			eat_flag = 0;
		pthread_mutex_unlock(&global->philo[i].m_times_eat);
		i++;
	}
	if (eat_flag)
	{
		pthread_mutex_lock(&global->m_stop_flag);
		global->stop_flag = 1;
		printf("Gluttony\n");
		pthread_mutex_unlock(&global->m_stop_flag);
		return (1);
	}
	return (0);
}

void	clean_global(t_global *global)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&global->m_printer);
	pthread_mutex_destroy(&global->m_stop_flag);
	while (i < global->philo_amount)
	{
		pthread_mutex_destroy(&global->philo[i].m_last_eat);
		pthread_mutex_destroy(&global->philo[i].m_times_eat);
		pthread_mutex_destroy(&global->forks[i]);
		i++;
	}
	free(global->forks);
	free(global->philo);
	return ;
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
		exit (1);
	if (create_philos(&global))
		exit (1);
	while (1)
		if (supervise(&global))
			break;
	printf("%lli Threads terminated\n", diff_time(global.start_time));
	while (i < global.philo_amount)
	{
		if (pthread_join(global.philo[i].self, NULL))
			exit (1);
		//printf("joined %i\n", global.philo[i].id);
		i++;
	}
	clean_global(&global);
	printf("%lli - TERMINATED ++++++++++++++\n\n", diff_time(global.start_time));
	return (0);
}
