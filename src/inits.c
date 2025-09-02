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

/*
	| Reserva e inicializa la estructura t_table.
	| Carga reglas desde av: N, t_die, t_eat, t_sleep.
	| Si ac==6, must_eat=av[5]; si no, must_eat=-1.
	| Reserva array de mutex para forks (N entradas).
	| Cada fork es un mutex: exclusión en tenedor.
	| Inicializa cada fork con pthread_mutex_init.
	| Crea mutex de impresión: print_mutex.
	| Crea mutex de estado de muerte: died_mutex.
	| start_time=0: se fijará luego para sincronizar.
	| died=0: bandera global, se protege con died_mutex.
	| Ownership: table libera forks y destruye mutex.
	| Si malloc de forks falla: free(table) y NULL.
	| Nota: no se comprueban errores al crear mutex.
	| Para robustez, maneja fallos parciales y cleanup.
	| Gancho: died_mutex permite un monitor de muerte.
	| Devuelve puntero listo o NULL si hubo error.
*/
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

/*
	| Inicializa el array de t_philosopher.
	| Tamaño: num_philosophers de la mesa.
	| Asigna id, forks, contador y punteros compartidos.
	| Forks: left=i, right=(i+1)%N (índices circulares).
	| last_meal = start_time para arrancar sincronizado.
	| meal_mutex protege last_meal y meals_eaten.
	| thread_id=0 hasta pthread_create lo rellene.
	| Conceptos:
	| - Índices mod N modelan la mesa circular.
	| - Mutex por filósofo evita carreras en stats.
	| - El orden de forks no evita deadlock por sí solo.
	|   Se resuelve en la rutina (p.ej. orden alterno).
	| Devuelve puntero o NULL si malloc falla.
*/
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

/*
	| Libera recursos por-filósofo y su mutex.
	| Destruye meal_mutex[i] antes de liberar el array.
	| Concepto: destruir mutex evita fugas de kernel.
	| Seguro si philos==NULL no se llama a esta función.
*/
void	free_philo(t_table *table, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < table->num_philosophers)
		pthread_mutex_destroy(&philos[i++].meal_mutex);
	free(philos);
}

/*
	| Libera la mesa y sus mutex compartidos.
	| Destruye todos los forks antes del free.
	| Destruye el mutex de impresión (print_mutex).
	| Luego libera el array forks y la propia table.
	| Mejora: también destruir died_mutex aquí.
	|   (falta pthread_mutex_destroy(&table->died_mutex);)
	| Ownership: la mesa es dueña de forks y mutex.
*/
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
