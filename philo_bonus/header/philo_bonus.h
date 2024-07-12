/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:37:57 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 12:58:03 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>

typedef enum s_action {
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE,
	INPUT_ERR,
	FORK_ERR,
	PHILO_ERR,
	SEM_NAME_ERR,
	MALLOC_ERR,
	SEM_OPEN_ERR
}	t_action;

// struct for given argument informations
typedef struct s_infos
{
	volatile size_t			last_eaten;
	volatile unsigned int	times_eaten;
	volatile char			forks_taken;
	volatile char			is_living;
	unsigned int			philo_id;
	unsigned int			nmb_philo;
	unsigned int			die_time;
	unsigned int			eat_time;
	unsigned int			sleep_time;
	unsigned int			nmb_eats;
	sem_t					*print_sem;
	sem_t					*forks_sem;
	sem_t					**action_locks;
	sem_t					**die_locks;
	pid_t					*dummys;
	pid_t					*philos;
	pid_t					eat_cycle;
	pthread_t				die_thread;
}	t_infos;

// SEMAPHORES NAMING
char	*ito_semname(unsigned int n, char opt);

// INIT FREE
char	handle_inputs(int argc, char **argv, t_infos *info);
char	init_forks_philos(t_infos *infos);
void	destroy_infos(t_infos *infos);

// UTILS
void	error_msg(t_action opt);
void	kill_all_childpr(t_infos *infos, pid_t pid);
char	ft_atoui(const char *string, unsigned int *result);
size_t	get_time(void);
void	printing_philostate(t_infos *infos, t_action action);

// CHECK ROUTINES
void	philo_routine(unsigned int id, t_infos *infos);

// PHILOSOPHER
void	create_philos(t_infos *infos);

#endif
