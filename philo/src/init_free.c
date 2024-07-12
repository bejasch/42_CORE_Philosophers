/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:17:29 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/10 14:20:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

static char	init_forks_actions(t_infos *infos, unsigned int id);
static char	set_philo(t_philo *philo, unsigned int id, t_infos *infos);

// initializes the forks
char	init_locks(t_infos *infos)
{
	infos->forks_inited = malloc(sizeof(char) * infos->nmb_philo);
	if (!infos->forks_inited)
		return (0);
	memset(infos->forks_inited, 0, sizeof(char) * infos->nmb_philo);
	infos->action_locks_inited = malloc(sizeof(char) * infos->nmb_philo);
	if (!infos->action_locks_inited)
		return (0);
	memset(infos->action_locks_inited, 0, sizeof(char) * infos->nmb_philo);
	infos->forks = malloc(sizeof(pthread_mutex_t) * infos->nmb_philo);
	if (!infos->forks)
		return (0);
	infos->action_locks = malloc(sizeof(pthread_mutex_t) * infos->nmb_philo);
	if (!infos->action_locks)
		return (0);
	if (pthread_mutex_init(&infos->print_lock, NULL))
		return (0);
	else
		infos->print_lock_inited = 1;
	return (init_forks_actions(infos, infos->nmb_philo));
}

static char	init_forks_actions(t_infos *infos, unsigned int id)
{
	while (id--)
	{
		if (pthread_mutex_init(&(infos->forks[id]), NULL))
			return (0);
		else
			infos->forks_inited[id] = 1;
		if (pthread_mutex_init(&(infos->action_locks[id]), NULL))
			return (0);
		else
			infos->action_locks_inited[id] = 1;
	}
	return (1);
}

// Initializes the philosophers and starts their threads
char	init_philos(t_infos *infos)
{
	unsigned int	id;

	infos->philos = (t_philo *)malloc(sizeof(t_philo) * infos->nmb_philo);
	if (!infos->philos)
		return (0);
	memset(infos->philos, 0, sizeof(t_philo) * infos->nmb_philo);
	infos->philo_thr = malloc(sizeof(pthread_t) * infos->nmb_philo);
	if (!infos->philo_thr)
		return (0);
	memset(infos->philo_thr, 0, sizeof(pthread_t) * infos->nmb_philo);
	id = infos->nmb_philo;
	while (id--)
	{
		if (!set_philo(infos->philos + id, id, infos))
			return (0);
		if (pthread_create(infos->philo_thr + id, NULL, philo_routine, \
			(void *)&infos->philos[id]))
			return (0);
	}
	return (1);
}

// sets the philosopher's values
static char	set_philo(t_philo *philo, unsigned int id, t_infos *infos)
{
	philo->id = id;
	philo->eat_time = infos->eat_time;
	philo->sleep_time = infos->sleep_time;
	philo->nmb_eats = infos->nmb_eats;
	philo->is_living = 1;
	if (id % 2 == 0)
	{
		philo->fst_fork = infos->forks + id;
		philo->snd_fork = infos->forks + ((id + 1) % infos->nmb_philo);
	}
	else
	{
		philo->fst_fork = infos->forks + ((id + 1) % infos->nmb_philo);
		philo->snd_fork = infos->forks + id;
	}
	philo->print_lock = &infos->print_lock;
	philo->action_lock = &infos->action_locks[id]; 
	philo->eat_start = get_time();
	return (1);
}

// checks and frees all allocated memory
void	destroy_infos(t_infos *infos)
{
	unsigned int	id;

	if (!infos->forks_inited)
		return ;
	if (!infos->forks)
		return (free(infos->forks_inited));
	id = infos->nmb_philo;
	while (id--)
	{
		if (infos->forks_inited[id])
			pthread_mutex_destroy(infos->forks + id);
		if (infos->action_locks_inited[id])
			pthread_mutex_destroy(infos->action_locks + id);
	}
	if (infos->print_lock_inited)
		pthread_mutex_destroy(&infos->print_lock);
	free(infos->forks);
	free(infos->forks_inited);
	free(infos->action_locks);
	free(infos->action_locks_inited);
	if (infos->philos)
		free(infos->philos);
	if (infos->philo_thr)
		free(infos->philo_thr);
}
