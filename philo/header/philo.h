/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:37:57 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 12:45:55 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>

typedef enum {
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}	t_action;

// struct for each philosophers information
typedef struct s_philo
{
	unsigned int	id;
	unsigned int	eat_time;
	unsigned int	sleep_time;
	unsigned int	nmb_eats;
	volatile char	is_living;
	volatile char	on_fork;
	volatile size_t	eat_start;
	pthread_mutex_t	*fst_fork;
	pthread_mutex_t	*snd_fork;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*action_lock;
}	t_philo;

// struct for given argument informations
typedef struct infos
{
	unsigned int	nmb_philo;
	unsigned int	die_time;
	unsigned int	eat_time;
	unsigned int	sleep_time;
	unsigned int	nmb_eats;
	pthread_mutex_t	*forks;
	char			*forks_inited;
	pthread_mutex_t	*action_locks;
	char			*action_locks_inited;
	pthread_mutex_t	print_lock;
	char			print_lock_inited;
	t_philo			*philos;
	pthread_t		*philo_thr;
}	t_infos;

// INITIATION & FREE
char	init_locks(t_infos *infos);
char	init_philos(t_infos *infos);
void	destroy_infos(t_infos *infos);
// CHECK ROUTINES
void	die_check(t_infos *infos, char active);
char	wait_for_death(t_philo *philo);
// UTILS
void	err_input(void);
char	ft_atoui(const char *string, unsigned int *result);
size_t	get_time(void);
void	printing_philostate(unsigned int nmb_philo, t_action action);
void	monitored_sleep(t_philo *philo, unsigned int time);
// PHILOSOPHERS
void	*philo_routine(void *input_philo);

#endif
