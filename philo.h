/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:51:48 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/06 13:54:05 by vperez-f         ###   ########.fr       */
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

# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define SLEEP "is sleeping\n"
# define THINK "is thinking\n"
# define DEATH "died\n"

typedef struct s_global
{
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

typedef struct s_philo
{
	int					id;
	int					times_eat;
	long long			last_eat;
	pthread_t			self;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		m_last_eat;
	pthread_mutex_t		m_times_eat;
	struct s_global		*global;
}	t_philo;

int			join_threads(t_global *global);
int			create_philos(t_global *glob);
int			create_forks(t_global *global);
int			invalid_args(int argc, char **argv);

size_t		ft_strlen(const char *str);

long long	get_time(void);
long long	diff_time(long long start_time);

void		*routine(void *args);
void		philo_sleep(t_philo *philo);
void		clean_global(t_global *global);
void		kill_philo(int id, t_global *global);
void		ft_sleep(int m_sec, t_global *global);
void		writer(char *mode, int id, t_global *global);
void		check_meals(t_global *global, int i, int *flag);
void		init_global(t_global *g, char **argv, int argc);

#endif