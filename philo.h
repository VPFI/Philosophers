/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:48 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/29 15:19:15 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DEATH 5

typedef struct s_global {
	int				philo_amount;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eating_needs;
	int				stop_flag;
	long long		start_time;
	struct s_philo	*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	m_printer;
	pthread_mutex_t	m_stop_flag;
}	t_global;

typedef struct s_philo {
	int					id;
	int					times_eat;
	long long			last_eat;
	pthread_t			self;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		m_last_eat;
	pthread_mutex_t		m_times_eat;
	struct	s_global	*global;
}	t_philo;

long long	diff_time(long long start_time);

#endif