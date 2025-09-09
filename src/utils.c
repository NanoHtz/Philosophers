/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:09:56 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/06/25 20:09:56 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

void	set_died(t_table *table)
{
	pthread_mutex_lock(&table->died_mutex);
	table->died = 1;
	pthread_mutex_unlock(&table->died_mutex);
}

long	get_time(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000) + ((tv.tv_usec / 1000));
	return (time);
}

long	ft_time(t_philosopher *philo)
{
	long	time;

	time = get_time() - philo->table->start_time;
	return (time);
}

void	print_action(t_philosopher *philo, char *msg)
{
	if (!philo || !philo->table)
	{
		printf("ERROR: print_action recibió philo o table NULL\n");
		return ;
	}
	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ld %d %s\n", ft_time(philo), philo->id + 1, msg);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	check_args(int ac, char **av)
{
	int		i;
	long	nbr;

	i = 1;
	if (ac != 5 && ac != 6)
		return (ft_perror(E_ARGS, -1));
	while (i < ac)
	{
		if (is_number(av[i]) != 0)
			return (ft_perror(E_SIGN, -1));
		nbr = ft_atol(av[i]);
		if ((nbr <= 0) || nbr > INT_MAX)
			return (ft_perror(E_RANGE, -1));
		if (i == 1 && nbr < 1)
			return (ft_perror(E_PHILO, -1));
		i++;
	}
	return (0);
}
