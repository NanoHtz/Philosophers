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

/*
	| Lee la bandera 'died' con exclusión mutua.
	| Devuelve 1 si sigue viva; 0 si ya terminó.
	| Uso: checks de vida sin condiciones de carrera.
	| Nota: lock breve; leer y desbloquear rápido.
*/
int	get_died(t_table *table)
{
	int	alive;

	pthread_mutex_lock(&table->died_mutex);
	alive = (table->died == 0);
	pthread_mutex_unlock(&table->died_mutex);
	return (alive);
}

/*
	| Lanza hilo monitor para vigilar la simulación.
	| Crea control_thread con control_routine.
	| Pasa 'philos' (base del array) como contexto.
	| detach: el hilo libera recursos al terminar.
	| Sin join: no hay espera de fin explícita.
	| Asegura que 'philos' y 'table' viven hasta el fin.
	| Conceptos: flag 'died' con mutex; metas de comida.
	| El monitor puede señalar parar a los filósofos.
	| Mejoras: comprobar retornos de pthread_create.
	| Si quieres cierre ordenado, evita detach.
	| Guarda id y haz join o usa cond vars.
	| Gancho: medir latencias o imprimir estado si debug.
	| control_routine debe salir al poner 'died'=1.
*/
void	start_control(t_philosopher	*philos)
{
	pthread_t	control_thread;

	pthread_create(&control_thread, NULL, control_routine, (void *)philos);
	pthread_detach(control_thread);
}

/*
	| Comprueba si todos alcanzaron 'must_eat'.
	| Recorre N filósofos y cuenta los 'full'.
	| Lee meals_eaten con meal_mutex (lectura segura).
	| Devuelve 1 si full==N; en otro caso devuelve 0.
	| Uso: se llama solo si must_eat>0.
	| Concepto: barrera por objetivo de comidas.
*/
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

/*
	| Detecta muerte si now-last_meal >= time_to_die.
	| Toma snapshot: now=get_time() al inicio del bucle.
	| last_meal se lee bajo meal_mutex (consistencia).
	| Si excede t_die: set_died y print_action("died").
	| Libera meal_mutex antes de devolver.
	| Supuesto: print_action usa print_mutex global.
	| Devuelve 1 si hubo muerte; 0 si nadie murió.
*/
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

/*
	| Hilo monitor: supervisa metas y muertes.
	| 'philos' es base del array; table=philos[0].table.
	| Bucle principal mientras la sim siga activa.
	| Si must_eat>0 y loop()==true: marca fin (set_died).
	| Si loop_2 detecta muerte: termina.
	| Pausa breve usleep(1000) para evitar busy-wait.
	| Nota: confirma semántica de get_died (activa/fin).
	| Mejora: cond vars o eventos en lugar de sleeps.
*/
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
