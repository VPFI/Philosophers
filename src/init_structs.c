/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 13:23:16 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:47:20 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	create_philos(t_global *glob)
{
	int	i;

	i = 0;
	glob->philo = (t_philo *)calloc(glob->philo_amount, sizeof(t_philo));
	if (!glob->philo)
		return (1);
	pthread_mutex_lock(&glob->m_stop_flag);
	while (!glob->stop_flag && i < glob->philo_amount)
	{
		glob->philo[i].global = glob;
		glob->philo[i].id = i + 1;
		glob->philo[i].left_fork = &glob->forks[i];
		pthread_mutex_init(&glob->philo[i].m_last_eat, NULL);
		pthread_mutex_init(&glob->philo[i].m_times_eat, NULL);
		if (i == 0)
			glob->philo[i].right_fork = &glob->forks[glob->philo_amount - 1];
		else
			glob->philo[i].right_fork = &glob->forks[i - 1];
		if (pthread_create(&(glob->philo[i].self), NULL,
				&routine, (void *)&glob->philo[i]))
			return (1);
		i++;
	}
	glob->start_time = get_time();
	return (pthread_mutex_unlock(&glob->m_stop_flag), 0);
}

int	create_forks(t_global *global)
{
	int	i;

	i = 0;
	global->forks = (pthread_mutex_t *)calloc(
			(global->philo_amount), sizeof(pthread_mutex_t));
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

static const char	*space(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
	{
		i++;
	}
	return (&str[i]);
}

static int	ft_atoi(const char *str)
{
	int	i;
	int	ans;
	int	sign;

	i = 0;
	ans = 0;
	sign = 1;
	str = space(str);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sign *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ans = ans * 10 + (str[i] - '0');
		i++;
	}
	return (ans * sign);
}

void	init_global(t_global *g, char **argv, int argc)
{
	g->philo_amount = ft_atoi(argv[1]);
	g->time_die = ft_atoi(argv[2]);
	g->time_eat = ft_atoi(argv[3]);
	g->time_sleep = ft_atoi(argv[4]);
	if (argc == 5)
		g->eating_needs = -1;
	else if (argc == 6)
		g->eating_needs = ft_atoi(argv[5]);
	g->stop_flag = 0;
	pthread_mutex_init(&g->m_printer, NULL);
	pthread_mutex_init(&g->m_stop_flag, NULL);
}
