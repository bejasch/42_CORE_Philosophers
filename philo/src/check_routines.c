/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:16:58 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/10 16:09:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

static char	philo_life_check(t_philo *philo, t_infos *infos, char *active);

// checks continuously if a philosopher dies
void	die_check(t_infos *infos, char active)
{
	unsigned int	id;

	while (active)
	{
		active = 0;
		id = infos->nmb_philo;
		while (id--)
		{
			if (!philo_life_check(&infos->philos[id], infos, &active))
			{
				active = 0;
				break ;
			}
		}
		usleep(500);
	}
	id = infos->nmb_philo;
	while (id--)
	{
		pthread_mutex_lock(infos->philos[id].action_lock);
		infos->philos[id].is_living = 0;
		pthread_mutex_unlock(infos->philos[id].action_lock);
	}
}

static char	philo_life_check(t_philo *philo, t_infos *infos, char *active)
{
	pthread_mutex_lock(philo->action_lock);
	if (philo->is_living)
	{
		if (get_time() - philo->eat_start > infos->die_time)
		{
			philo->is_living = 0;
			pthread_mutex_lock(philo->print_lock);
			printing_philostate(philo->id, DIE);
			pthread_mutex_unlock(philo->print_lock);
			if (!infos->nmb_eats)
			{
				pthread_mutex_unlock(philo->action_lock);
				return (0);
			}
		}
		else if (!infos->nmb_eats || philo->nmb_eats)
			*active = 1;
	}
	pthread_mutex_unlock(philo->action_lock);
	return (1);
}

// Happens only if there is one philosopher / fork
char	wait_for_death(t_philo *philo)
{
	while (1)
	{
		if (!philo->is_living)
			return (0);
		pthread_mutex_unlock(philo->action_lock);
		usleep(1000);
		pthread_mutex_lock(philo->action_lock);
	}
	return (0);
}
