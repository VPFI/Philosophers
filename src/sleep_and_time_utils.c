/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_and_time_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 16:09:58 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:54:06 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

long long	get_time(void)
{
	static struct timeval	tv;
	long long				res;

	gettimeofday(&tv, NULL);
	res = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (res);
}

long long	diff_time(long long start_time)
{
	long long	res;

	res = -1;
	res = get_time() - start_time;
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

void	philo_sleep(t_philo *philo)
{
	writer(SLEEP, philo->id, philo->global);
	ft_sleep(philo->global->time_sleep, philo->global);
}
