/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:36:40 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/07/01 11:36:40 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

int	get_died(t_table *table)
{
	int	alive;

	pthread_mutex_lock(&table->died_mutex);
	alive = (table->died == 0);
	pthread_mutex_unlock(&table->died_mutex);
	return (alive);
}

void	start_control(t_philosopher	*philos)
{
	pthread_t	control_thread;

	pthread_create(&control_thread, NULL, control_routine, (void *)philos);
	pthread_detach(control_thread);
}

int	loop(t_philosopher *philos, t_table *table)
{
	int	i;
	int	full;

	i = 0;
	full = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals_eaten >= table->must_eat)
			full++;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	return (full == table->num_philosophers);
}

int	loop_2(t_philosopher *philos, t_table *table)
{
	int		i;
	long	now;

	now = get_time();
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (now - philos[i].last_meal >= table->time_to_die)
		{
			set_died(table);
			print_action(&philos[i], "died");
			pthread_mutex_unlock(&philos[i].meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	return (0);
}

void	*control_routine(void *arg)
{
	t_philosopher	*philos;
	t_table			*table;

	philos = (t_philosopher *)arg;
	table = philos[0].table;
	while (get_died(table))
	{
		if (table->must_eat > 0 && loop(philos, table))
		{
			set_died(table);
			return (NULL);
		}
		if (loop_2(philos, table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
