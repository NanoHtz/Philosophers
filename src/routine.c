/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 12:24:16 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/08/29 12:24:16 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

void	*one_philo(t_philosopher *philo, t_table *t)
{
	long	start;

	start = philo->last_meal;
	lock_fork(philo, philo->left_fork);
	while (get_time() - start < t->time_to_die)
		usleep(100);
	print_action(philo, "died");
	set_died(t);
	return (NULL);
}

void	stagger_if_odd(t_philosopher *philo)
{
	t_table	*t;
	long	start;

	t = philo->table;
	if ((t->num_philosophers % 2) == 1 && (philo->id % 2) == 0)
	{
		start = get_time();
		while (get_died(t) && get_time() - start < (t->time_to_eat / 2))
			usleep(100);
	}
}

void	stagger_even_first_turn(t_philosopher *philo)
{
	t_table	*t;
	long	start;

	t = philo->table;
	if ((t->num_philosophers % 2) == 0
		&& (philo->id % 2) == 1
		&& philo->meals_eaten == 0)
	{
		start = get_time();
		while (get_died(t) && get_time() - start < (t->time_to_eat / 2))
			usleep(100);
	}
}

void	routine_loop(t_philosopher	*philo)
{
	stagger_even_first_turn(philo);
	stagger_if_odd(philo);
	take_forks(philo);
	eat(philo);
	unlock_forks(philo);
	sleep_and_think(philo);
}

void	*routine(void *arg)
{
	t_philosopher	*philo;
	t_table			*t;

	philo = arg;
	t = philo->table;
	while (get_time() < t->start_time && get_died(t))
		usleep(100);
	while (get_died(t))
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (t->must_eat >= 0
			&& philo->meals_eaten >= t->must_eat)
		{
			pthread_mutex_unlock(&philo->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->meal_mutex);
		if (t->num_philosophers == 1)
			return (one_philo(philo, t));
		routine_loop(philo);
	}
	return (NULL);
}
