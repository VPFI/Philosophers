/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:26:30 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:48:08 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	eat_odd(t_philo *philo)
{
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_lock(philo->right_fork);
	writer(FORK, philo->id, philo->global);
	writer(EAT, philo->id, philo->global);
	pthread_mutex_lock(&philo->m_times_eat);
	philo->times_eat += 1;
	pthread_mutex_unlock(&philo->m_times_eat);
	ft_sleep(philo->global->time_eat, philo->global);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	eat_even(t_philo *philo)
{
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->right_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	writer(EAT, philo->id, philo->global);
	pthread_mutex_lock(&philo->m_times_eat);
	philo->times_eat += 1;
	pthread_mutex_unlock(&philo->m_times_eat);
	ft_sleep(philo->global->time_eat, philo->global);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

static void	single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->m_stop_flag);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->m_last_eat);
	pthread_mutex_unlock(&philo->global->m_stop_flag);
	writer(THINK, philo->id, philo->global);
	pthread_mutex_lock(philo->left_fork);
	writer(FORK, philo->id, philo->global);
	pthread_mutex_unlock(philo->left_fork);
	ft_sleep(philo->global->time_die, philo->global);
	ft_sleep(philo->global->time_die, philo->global);
	return ;
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = args;
	if (philo->global->philo_amount == 1)
	{
		single_philo_routine(philo);
		return (NULL);
	}
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
	return (NULL);
}
