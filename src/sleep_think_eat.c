/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_and_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:32:40 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/07/01 10:32:40 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

void	sleep_and_think(t_philosopher *philo)
{
	long	start;

	if (get_died(philo->table))
		print_action(philo, "is sleeping");
	start = get_time();
	while (get_died(philo->table)
		&& get_time() - start < philo->table->time_to_sleep)
		usleep(100);
	if (get_died(philo->table))
		print_action(philo, "is thinking");
}

void	eat(t_philosopher *philo)
{
	t_table	*t;
	long	start;
	int		do_print;

	t = philo->table;
	start = get_time();
	do_print = 0;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = start;
	if (get_died(t))
	{
		philo->meals_eaten++;
		do_print = 1;
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	if (do_print)
		print_action(philo, "is eating");
	while (get_died(t) && get_time() - start < t->time_to_eat)
		usleep(100);
}
