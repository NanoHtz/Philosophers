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

/*
	| Gestiona dormir y pensar del filósofo.
	| Si la sim vive, imprime "is sleeping".
	| Espera t_sleep con pausas cortas (usleep(100)).
	| Luego, si sigue viva, imprime "is thinking".
	| get_died evita logs cuando la sim ya acabó.
	| Gancho: cambiar a cond vars para menor CPU.
*/
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

/*
	| Inicia comida: marca tiempo y actualiza estado.
	| meal_mutex protege last_meal y meals_eaten.
	| Solo si la sim vive incrementa meals_eaten.
	| do_print decide el log fuera del mutex (menos lock).
	| Nota: si muere tras unlock, el log ya se decidió.
	| Espera t_eat con pausas cortas (usleep(100)).
	| Concepto: doble check de vida al comer.
	| Gancho: usar cond vars en lugar de sondeo.
*/
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
