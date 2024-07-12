/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:17:16 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 12:40:26 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

static char	taking_forks(t_philo *philo);
static char	eating(t_philo *philo);
static char	sleeping(t_philo *philo);
static char	thinking(t_philo *philo);

// The philosophers alternatively eat, think, or sleep.
void	*philo_routine(void *input_philo)
{
	t_philo		*philo;

	philo = (t_philo *)input_philo;
	pthread_mutex_lock(philo->action_lock);
	philo->eat_start = get_time();
	pthread_mutex_lock(philo->print_lock);
	printing_philostate(philo->id, THINK);
	pthread_mutex_unlock(philo->print_lock);
	pthread_mutex_unlock(philo->action_lock);
	while (1)
	{
		if (!taking_forks(philo) 
			|| !eating(philo)
			|| !sleeping(philo)
			|| !thinking(philo))
			break ;
	}
	if (philo->on_fork > 1)
		pthread_mutex_unlock(philo->snd_fork);
	if (philo->on_fork > 0)
		pthread_mutex_unlock(philo->fst_fork);
	pthread_mutex_unlock(philo->action_lock);
	return (NULL);
}

// Philosopher takes both forks individually with deadlock prevention
static char	taking_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->action_lock);
	if (!philo->is_living)
		return (0);
	philo->on_fork = 1;
	pthread_mutex_unlock(philo->action_lock);
	pthread_mutex_lock(philo->fst_fork);
	pthread_mutex_lock(philo->action_lock);
	if (!philo->is_living)
		return (0);
	pthread_mutex_lock(philo->print_lock);
	printing_philostate(philo->id, FORK);
	pthread_mutex_unlock(philo->print_lock);
	if (philo->fst_fork == philo->snd_fork)
		return (wait_for_death(philo));
	philo->on_fork = 2;
	pthread_mutex_unlock(philo->action_lock);
	pthread_mutex_lock(philo->snd_fork);
	return (1);
}

// Philosopher eats for eat_time ms
static char	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->action_lock);
	if (!philo->is_living)
		return (0);
	pthread_mutex_lock(philo->print_lock);
	printing_philostate(philo->id, FORK);
	printing_philostate(philo->id, EAT);
	pthread_mutex_unlock(philo->print_lock);
	philo->eat_start = get_time();
	pthread_mutex_unlock(philo->action_lock);
	monitored_sleep(philo, philo->eat_time);
	return (1);
}

// Philosopher sleeps for sleep_time ms
static char	sleeping(t_philo *philo)
{
	pthread_mutex_lock(philo->action_lock);
	if (!philo->is_living)
		return (0);
	pthread_mutex_unlock(philo->snd_fork);
	pthread_mutex_unlock(philo->fst_fork);
	philo->on_fork = 0;
	if (philo->nmb_eats)
		philo->nmb_eats--;
	pthread_mutex_lock(philo->print_lock);
	printing_philostate(philo->id, SLEEP);
	pthread_mutex_unlock(philo->print_lock);
	pthread_mutex_unlock(philo->action_lock);
	monitored_sleep(philo, philo->sleep_time);
	return (1);
}

// Philosopher thinks
static char	thinking(t_philo *philo)
{
	pthread_mutex_lock(philo->action_lock);
	if (!philo->is_living)
		return (0);
	pthread_mutex_lock(philo->print_lock);
	printing_philostate(philo->id, THINK);
	pthread_mutex_unlock(philo->print_lock);
	pthread_mutex_unlock(philo->action_lock);
	return (1);
}
