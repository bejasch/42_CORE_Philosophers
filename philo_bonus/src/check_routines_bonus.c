/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_routines_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:30:52 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/11 11:04:39 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

static void	*end_life(t_infos *infos, unsigned int id);
static void	*die_routine(void *input_infos);
static char	taking_forks(unsigned int id, t_infos *infos);
static char	eat_sleep_think(unsigned int id, t_infos *infos);

// The philosophers alternatively eat, think, or sleep.
void	philo_routine(unsigned int id, t_infos *infos)
{
	sem_wait(infos->die_locks[id]);
	infos->philo_id = id;
	if (pthread_create(&infos->die_thread, NULL, die_routine, (void *)infos))
		return ;
	infos->last_eaten = get_time();
	printing_philostate(infos, THINK);
	sem_post(infos->die_locks[id]);
	while (1)
		if (!taking_forks(id, infos)
			|| !eat_sleep_think(id, infos))
			break ;
	if (pthread_join(infos->die_thread, NULL))
		return ;
}

// Kills the id- / all the dummy processes (depending on nmb_eats).
// It signals, that this/all philosopher/s is/are done and it's processes 
// can be killed. This stops the program (simulation).
static void	*end_life(t_infos *infos, unsigned int id)
{
	sem_post(infos->action_locks[id]);
	infos->is_living = 0;
	printing_philostate(infos, DIE);
	if (infos->forks_taken)
		sem_post(infos->forks_sem);
	if (infos->forks_taken > 1)
		sem_post(infos->forks_sem);
	sem_post(infos->die_locks[id]);
	if (infos->nmb_eats)
		kill(infos->dummys[id], SIGKILL);
	else
	{
		id = infos->nmb_philo;
		while (id--)
			kill(infos->dummys[id], SIGKILL);
	}
	return (NULL);
}

// Thread, that checks the dead of a philosopher every ms.
// If so, it gives back recources and ends its process.
static void	*die_routine(void *input_infos)
{
	t_infos			*infos;
	unsigned int	id;

	infos = (t_infos *)input_infos;
	id = infos->philo_id;
	while (1)
	{
		sem_wait(infos->die_locks[id]);
		if (get_time() - infos->last_eaten > infos->die_time)
			return (end_life(infos, id));
		sem_post(infos->die_locks[id]);
		usleep(1000);
	}
	return (NULL);
}

// Process of taking forks shielde by semaphores
static char	taking_forks(unsigned int id, t_infos *infos)
{
	sem_wait(infos->action_locks[id]);
	sem_wait(infos->forks_sem);
	sem_wait(infos->die_locks[id]);
	if (!infos->is_living)
	{
		sem_post(infos->forks_sem);
		return (0);
	}
	infos->forks_taken++;
	printing_philostate(infos, FORK);
	sem_post(infos->die_locks[id]);
	sem_wait(infos->forks_sem);
	sem_wait(infos->die_locks[id]);
	if (!infos->is_living)
	{
		sem_post(infos->forks_sem);
		return (0);
	}
	infos->forks_taken++;
	printing_philostate(infos, FORK);
	sem_post(infos->die_locks[id]);
	sem_post(infos->action_locks[id]);
	return (1);
}

// Process of eating, sleeping, thinking shielded by semaphores
static char	eat_sleep_think(unsigned int id, t_infos *infos)
{
	sem_wait(infos->die_locks[id]);
	printing_philostate(infos, EAT);
	infos->last_eaten = get_time();
	sem_post(infos->die_locks[id]);
	usleep(1000 * infos->eat_time);
	sem_wait(infos->die_locks[id]);
	if (!infos->is_living)
		return (0);
	sem_post(infos->forks_sem);
	sem_post(infos->forks_sem);
	infos->forks_taken = 0;
	if (infos->nmb_eats && ++infos->times_eaten == infos->nmb_eats)
		kill(infos->dummys[id], SIGKILL);
	printing_philostate(infos, SLEEP);
	sem_post(infos->die_locks[id]);
	usleep(1000 * infos->sleep_time);
	sem_wait(infos->die_locks[id]);
	if (!infos->is_living)
		return (0);
	printing_philostate(infos, THINK);
	sem_post(infos->die_locks[id]);
	return (1);
}
