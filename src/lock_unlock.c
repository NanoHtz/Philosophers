/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:25:28 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/08/30 13:25:28 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

void	unlock_fork(t_philosopher *philo, int fork_index)
{
	pthread_mutex_unlock(&philo->table->forks[fork_index]);
}

int	lock_fork(t_philosopher *philo, int fork_index)
{
	if (!get_died(philo->table))
		return (0);
	pthread_mutex_lock(&philo->table->forks[fork_index]);
	if (!get_died(philo->table))
	{
		pthread_mutex_unlock(&philo->table->forks[fork_index]);
		return (0);
	}
	print_action(philo, "has taken a fork");
	return (1);
}

void	unlock_forks(t_philosopher *philo)
{
	if (philo->table->num_philosophers == 1)
	{
		unlock_fork(philo, philo->left_fork);
		return ;
	}
	if (philo->id % 2 == 0)
	{
		unlock_fork(philo, philo->left_fork);
		unlock_fork(philo, philo->right_fork);
	}
	else
	{
		unlock_fork(philo, philo->right_fork);
		unlock_fork(philo, philo->left_fork);
	}
}
