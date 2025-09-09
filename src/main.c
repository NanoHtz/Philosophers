/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:10:05 by fgalvez-          #+#    #+#             */
/*   Updated: 2024/12/05 15:14:50 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

void	create_and_join(t_table *table, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_create(&philos[i].thread_id, NULL, routine, &philos[i]);
		usleep(200);
		i++;
	}
	usleep(1000);
	start_control(philos);
	i = 0;
	while (i < table->num_philosophers)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_table			*table;
	t_philosopher	*philos;

	if (check_args(ac, av) == -1)
		return (-1);
	table = init_table(ac, av);
	if (!table)
		return (ft_perror("Error al inicializar la mesa.", -1));
	table->start_time = get_time() + 100;
	philos = init_philosophers(table);
	if (!philos)
	{
		free_table(table);
		return (ft_perror("Error al inicializar los filósofos.", -1));
	}
	create_and_join(table, philos);
	free_philo(table, philos);
	free_table(table);
	return (0);
}
