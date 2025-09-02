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

/*
	| Orquesta hilos: crea, arranca control y hace join.
	| Crea N hilos con routine y pasa &philos[i].
	| usleep(200) escalona arranques y baja contención.
	| Pausa extra: usleep(1000) antes de start_control.
	| start_control vigila muerte y fin de criterios.
	| Luego espera a todos con pthread_join.
	|
	| Conceptos:
	| - Escalonar reduce choques en mutex de forks.
	| - El monitor puede parar vía 'died' compartido.
	| - join asegura cierre ordenado y limpieza.
	| - Si algo no parece crítico, es gancho de ajuste.
	|
	| Mejoras futuras:
	| - Comprobar retornos de create y de join.
	| - Mejor que sleeps: barrera con start_time
	|   o cond var para sincronía precisa.
*/
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
/*
	| Entrada del programa: orquesta la simulación.
	| Pasos:
	| 1) Valida los argumentos.
	| 2) Crea la mesa (tiempos, mutex de forks/print).
	| 3) Fija start_time en el futuro para sincronizar.
	| 4) Crea filósofos (id, forks asignados, reglas).
	| 5) Crea hilos, espera y recoge su final (join).
	| 6) Libera memoria y destruye mutex.
	|
	| Conceptos:
	| - mutex de fork: evita uso del mismo tenedor a la vez.
	| - mutex de print: evita mezcla en la salida estándar.
	| - start_time: barrera para iniciar todos a la vez.
	|
	| Diseño:
	| - get_time()+100 da ~100 ms de margen de arranque.
	|   Reduce sesgos por latencias al crear hilos.
	| - Si algo hoy parece "vacío", es gancho para extras
	|   (métricas, trazas, reglas adicionales).
*/
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
