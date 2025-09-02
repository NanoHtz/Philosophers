/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:09:56 by fgalvez-          #+#    #+#             */
/*   Updated: 2025/06/25 20:09:56 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Inc/philosophers.h"

/*
	| Marca la simulación como terminada.
	| Escribe died=1 bajo protección de died_mutex.
	| Concepto: flag atómico para parar hilos.
*/
void	set_died(t_table *table)
{
	pthread_mutex_lock(&table->died_mutex);
	table->died = 1;
	pthread_mutex_unlock(&table->died_mutex);
}

/*
	| Devuelve tiempo actual en milisegundos.
	| Usa gettimeofday y compone tv_sec y tv_usec.
	| Nota: no es reloj monotónico estricto.
	|   Si se permite, preferir CLOCK_MONOTONIC.
*/
long	get_time(void)
{
	struct timeval	tv;
	long			time;

	gettimeofday(&tv, NULL);
	time = (tv.tv_sec * 1000) + ((tv.tv_usec / 1000));
	return (time);
}

/*
	| Tiempo relativo desde start_time en ms.
	| Útil para estampar logs coherentes.
	| No depende del reloj absoluto del sistema.
*/
long	ft_time(t_philosopher *philo)
{
	long	time;

	time = get_time() - philo->table->start_time;
	return (time);
}

/*
	| Imprime una acción con tiempo e id.
	| Formato: "<ms> <id> <msg>\n".
	| Protege la salida con print_mutex (atomiza).
	| Comprueba punteros nulos y avisa si fallan.
	| Nota: no filtra por 'died'; decide el llamante.
*/
void	print_action(t_philosopher *philo, char *msg)
{
	if (!philo || !philo->table)
	{
		printf("ERROR: print_action recibió philo o table NULL\n");
		return ;
	}
	pthread_mutex_lock(&philo->table->print_mutex);
	printf("%ld %d %s\n", ft_time(philo), philo->id + 1, msg);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

/*
	| Valida argumentos y rangos de entrada.
	| Espera 4 o 5 valores: ac debe ser 5 o 6.
	| is_number(av[i]) debe dar 0 (numérico válido).
	| Convierte con ft_atol; acepta 1..INT_MAX.
	| i==1 es nº de filósofos; debe ser >= 1.
	| E_SIGN si hay signos o chars inválidos.
	| E_RANGE si <= 0 o > INT_MAX.
	| E_PHILO es gancho para error específico.
	| Hoy no se alcanza por la regla previa (<=0).
	| Útil si reordenas validaciones para mensaje claro.
	| Retorna 0 si ok; -1 al primer fallo.
*/
int	check_args(int ac, char **av)
{
	int		i;
	long	nbr;

	i = 1;
	if (ac != 5 && ac != 6)
		return (ft_perror(E_ARGS, -1));
	while (i < ac)
	{
		if (is_number(av[i]) != 0)
			return (ft_perror(E_SIGN, -1));
		nbr = ft_atol(av[i]);
		if ((nbr <= 0) || nbr > INT_MAX)
			return (ft_perror(E_RANGE, -1));
		if (i == 1 && nbr < 1)
			return (ft_perror(E_PHILO, -1));
		i++;
	}
	return (0);
}
