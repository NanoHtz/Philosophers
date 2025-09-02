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

/*
	| Libera el mutex del fork indicado.
	| Helper pequeño para centralizar unlock.
	| Supone que el fork estaba bloqueado por philo.
*/
void	unlock_fork(t_philosopher *philo, int fork_index)
{
	pthread_mutex_unlock(&philo->table->forks[fork_index]);
}

/*
	| Toma un fork si la simulación sigue viva.
	| Early-exit: si get_died()==0, no bloquea.
	| Tras lock revalida vida; si murió, suelta y sale.
	| Imprime "has taken a fork" si tuvo éxito.
	| Doble check evita prints tras el fin.
	| Devuelve 1 si tomó el fork; 0 si no.
	| El orden de forks lo decide la rutina externa.
*/
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

/*
	| Suelta los forks del filósofo con orden estable.
	| Caso N==1: solo suelta el izquierdo y retorna.
	| Par: left luego right. Impar: right luego left.
	| Orden simétrico por id para consistencia.
	| Gancho: aquí puedes variar la estrategia.
*/
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
