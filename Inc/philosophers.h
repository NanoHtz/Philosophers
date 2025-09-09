/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgalvez- <fgalvez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:16:53 by fgalvez-          #+#    #+#             */
/*   Updated: 2024/12/05 15:09:58 by fgalvez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

# define E_ARGS "wrong number of arguments. Only 4 or 5 arguments"
# define E_SIGN "Only digits."
# define E_RANGE "Out of range (1-INT_MAX)."
# define E_PHILO "First argument must be > 1."
# define E_EMPTY "The string is empty"

typedef struct s_table
{
	int				died;
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	died_mutex;
	long			start_time;
}	t_table;

typedef struct s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	long			last_meal;
	int				meals_eaten;
	pthread_mutex_t	meal_mutex;
	t_table			*table;
	pthread_t		thread_id;
}	t_philosopher;

//inits.c
t_table			*init_table(int ac, char **av);
t_philosopher	*init_philosophers(t_table *table);
void			free_philo(t_table *table, t_philosopher *philos);
void			free_table(t_table *table);
//routine.c
void			*routine(void *arg);
void			routine_loop(t_philosopher	*philo);
void			stagger_even_first_turn(t_philosopher *philo);
void			stagger_if_odd(t_philosopher *philo);
void			*one_philo(t_philosopher *philo, t_table *t);
//control.c
int				get_died(t_table *table);
void			start_control(t_philosopher	*philos);
void			*control_routine(void *arg);
int				loop(t_philosopher *philos, t_table *table);
int				loop_2(t_philosopher *philos, t_table *table);
//sleep_think_eat.c
void			sleep_and_think(t_philosopher *philo);
void			eat(t_philosopher *philo);
//take_forks.c
void			unlock_fork(t_philosopher *philo, int fork_index);
int				lock_fork(t_philosopher *philo, int fork_index);
void			take_forks(t_philosopher *philo);
void			unlock_forks(t_philosopher *philo);
//utils.c
void			set_died(t_table *table);
long			get_time(void);
long			ft_time(t_philosopher *philo);
void			print_action(t_philosopher *philo, char *msg);
int				check_args(int ac, char **av);

#endif
