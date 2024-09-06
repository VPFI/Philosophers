/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:21:25 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/02 16:10:15 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

long long	ft_atol(const char *str)
{
	int			i;
	long long	sign;
	long long	ans;

	i = 0;
	ans = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ans = ans * 10 + (str[i] - '0');
		i++;
	}
	ans *= sign;
	return (ans);
}

int	is_integer(char *num)
{
	int			length;
	int			i;
	long long	res;

	i = 0;
	if (num[i] == '-')
		i++;
	while (num[i] && num[i] == '0')
		i++;
	length = ft_strlen(&num[i]);
	if (length > 11)
		return (0);
	res = ft_atol(num);
	if (res < INT_MIN || INT_MAX < res)
		return (0);
	return (1);
}

int	valid_num(char *num)
{
	int	i;

	i = 0;
	if (num[i] == '+' || num[i] == '-')
		i++;
	while (num[i])
	{
		if (!ft_isdigit(num[i]))
			return (0);
		i++;
	}
	return (1);
}

int	invalid_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (argv[i] && i < argc)
	{
		if (!valid_num(argv[i]) || !is_integer(argv[i]))
			return (1);
		i++;
	}
	return (0);
}
