/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:48 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/24 16:58:49 by vperez-f         ###   ########.fr       */
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

typedef struct s_global {
	int				philo_amount;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eating_needs;
	int				death_flag;
	struct s_philo	*philo;
	pthread_mutex_t	*forks;
}	t_global;

typedef struct s_philo {
	int					id;
	int					times_eat;
	int					last_eat;
	pthread_t			self;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct	s_global	*global;
}	t_philo;

#endif