/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:33:29 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/06/24 11:33:29 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

t_table	*init_table(int ac, char **av)
{
	t_table	*table;
	int		i;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	table->died = 0;
	table->start_time = 0;
	table->num_philosophers = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->must_eat = ft_atoi(av[5]);
	else
		table->must_eat = -1;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philosophers);
	if (!table->forks)
		return (free(table), NULL);
	i = 0;
	while (i < table->num_philosophers)
		pthread_mutex_init(&table->forks[i++], NULL);
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->died_mutex, NULL);
	return (table);
}

t_philosopher	*init_philosophers(t_table *table)
{
	t_philosopher	*philos;
	int				i;

	philos = malloc(sizeof(t_philosopher) * table->num_philosophers);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < table->num_philosophers)
	{
		philos[i].id = i;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % table->num_philosophers;
		philos[i].meals_eaten = 0;
		philos[i].table = table;
		philos[i].last_meal = table->start_time;
		philos[i].thread_id = 0;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		i++;
	}
	return (philos);
}

void	free_philo(t_table *table, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
		pthread_mutex_destroy(&philos[i++].meal_mutex);
	free(philos);
}

void	free_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
		pthread_mutex_destroy(&table->forks[i++]);
	pthread_mutex_destroy(&table->print_mutex);
	free(table->forks);
	free(table);
}
