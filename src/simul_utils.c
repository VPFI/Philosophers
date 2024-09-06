/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simul_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:28:21 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:48:17 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	check_meals(t_global *global, int i, int *flag)
{
	if (global->eating_needs == -1)
		return ;
	pthread_mutex_lock(&global->philo[i].m_times_eat);
	if (global->philo[i].times_eat < global->eating_needs)
		*flag = 0;
	pthread_mutex_unlock(&global->philo[i].m_times_eat);
}

void	kill_philo(int id, t_global *global)
{
	long long	time;

	time = diff_time(global->start_time);
	pthread_mutex_lock(&global->m_stop_flag);
	global->stop_flag = 1;
	pthread_mutex_unlock(&global->m_stop_flag);
	if (id != -1)
	{
		pthread_mutex_lock(&global->m_printer);
		printf("%lli %i %s", time, id, DEATH);
		pthread_mutex_unlock(&global->m_printer);
	}
}

void	writer(char *mode, int id, t_global *global)
{
	long long	time;

	time = diff_time(global->start_time);
	pthread_mutex_lock(&global->m_printer);
	pthread_mutex_lock(&global->m_stop_flag);
	if (global->stop_flag)
	{
		pthread_mutex_unlock(&global->m_printer);
		pthread_mutex_unlock(&global->m_stop_flag);
		return ;
	}
	pthread_mutex_unlock(&global->m_stop_flag);
	printf("%lli %i %s", time, id, mode);
	pthread_mutex_unlock(&global->m_printer);
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

int	join_threads(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->philo_amount)
	{
		if (global->philo_amount == 1)
		{
			if (pthread_join(global->philo[i].self, NULL))
				return (1);
		}
		if (pthread_join(global->philo[i].self, NULL))
			return (1);
		i++;
	}
	return (0);
}
