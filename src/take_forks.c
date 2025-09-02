/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:00:05 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/06/25 20:00:05 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

/*
	| Orden de forks cuando N es par.
	| Pares: left luego right.
	| Impares: right luego left.
	| Rompe simetrías para evitar deadlock.
	| f1 y f2 salen listos para lock.
*/
void	set_even_order(t_philosopher *p, int *f1, int *f2)
{
	if (p->id % 2 == 0)
	{
		*f1 = p->left_fork;
		*f2 = p->right_fork;
	}
	else
	{
		*f1 = p->right_fork;
		*f2 = p->left_fork;
	}
}

/*
	| Orden de forks cuando N es impar.
	| Toma primero el de menor índice.
	| Impone orden global (jerarquía de recursos).
	| Evita espera circular y deadlock.
*/
static void	set_odd_order(t_philosopher *p, int *f1, int *f2)
{
	if (p->left_fork < p->right_fork)
	{
		*f1 = p->left_fork;
		*f2 = p->right_fork;
	}
	else
	{
		*f1 = p->right_fork;
		*f2 = p->left_fork;
	}
}

/*
	| Elige la estrategia según paridad de N.
	| N par: alterno por id (set_even_order).
	| N impar: orden total por índice (set_odd_order).
*/
static void	set_fork_order(t_philosopher *p, int *f1, int *f2)
{
	if ((p->table->num_philosophers % 2) == 0)
		set_even_order(p, f1, f2);
	else
		set_odd_order(p, f1, f2);
}

static int	acquire_both_forks(t_philosopher *p, int f1, int f2)
{
	t_table	*t;

	t = p->table;
	while (get_died(t))
	{
		if (!lock_fork(p, f1))
			return (0);
		if (pthread_mutex_trylock(&t->forks[f2]) == 0)
		{
			if (!get_died(t))
			{
				pthread_mutex_unlock(&t->forks[f2]);
				pthread_mutex_unlock(&t->forks[f1]);
				return (0);
			}
			print_action(p, "has taken a fork");
			return (1);
		}
		pthread_mutex_unlock(&t->forks[f1]);
		usleep(800);
	}
	return (0);
}

/*
	| Intenta tomar f1 y f2 con backoff.
	| Lock de f1 con lock_fork (valida vida).
	| f2 con trylock; si falla, suelta f1 y reintenta.
	| Tras tomar f2, revalida vida y loguea.
	| Backoff usleep(800) para bajar contención.
	| Devuelve 1 si tiene ambos; 0 si no.
*/
void	take_forks(t_philosopher *philo)
{
	int		f1;
	int		f2;
	t_table	*t;

	t = philo->table;
	if (t->num_philosophers == 1)
	{
		lock_fork(philo, philo->left_fork);
		return ;
	}
	set_fork_order(philo, &f1, &f2);
	if (!acquire_both_forks(philo, f1, f2))
		return ;
}
