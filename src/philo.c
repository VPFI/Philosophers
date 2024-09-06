/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:46 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:29:27 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	supervise(t_global *global)
{
	int	i;
	int	eat_flag;

	i = 0;
	eat_flag = 1;
	while (i < global->philo_amount)
	{
		pthread_mutex_lock(&global->philo[i].m_last_eat);
		if (global->philo[i].last_eat > 0
			&& (get_time() - global->philo[i].last_eat) > global->time_die)
		{
			pthread_mutex_unlock(&global->philo[i].m_last_eat);
			kill_philo(global->philo[i].id, global);
			return (1);
		}
		pthread_mutex_unlock(&global->philo[i].m_last_eat);
		check_meals(global, i, &eat_flag);
		i++;
	}
	if (eat_flag && global->eating_needs > -1)
	{
		kill_philo(-1, global);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_global	global;

	if (argc < 5 || argc > 6 || invalid_args(argc, argv))
		return (1);
	init_global(&global, argv, argc);
	if (create_forks(&global))
		return (1);
	if (create_philos(&global))
		return (1);
	while (1)
	{
		if (supervise(&global))
			break ;
	}
	join_threads(&global);
	clean_global(&global);
	return (0);
}
