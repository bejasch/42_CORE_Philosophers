/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 19:48:48 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 12:54:43 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	err_input(void)
{
	write(1, "Wrong inputs!\n", 15);
}

// tansforms a string to an unsigned int (result), returns 1 on success 
char	ft_atoui(const char *string, unsigned int *result)
{
	unsigned long	res;

	if (!string || !*string)
		return (0);
	res = 0;
	while (*string)
	{
		if (*string < '0' || *string > '9')
			return (0);
		res = 10 * res + *string - '0';
		if (res > (unsigned long)UINT_MAX)
			return (0);
		string++;
	}
	*result = (unsigned int)res;
	return (1);
}

// returns the actual time (of day) in ms
size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((size_t)tv.tv_sec * 1000 + (size_t)tv.tv_usec / 1000);
}

// Prints the current state of a philosopher.
// Timestamps are given in ms since the epoch !
void	printing_philostate(unsigned int nmb_philo, t_action action)
{
	nmb_philo++;
	if (action == FORK)
		printf("\033[35m%zu %u has taken a fork\033[0m\n", get_time(), nmb_philo);
	else if (action == EAT)
		printf("\033[36m%zu %u is eating\033[0m\n", get_time(), nmb_philo);
	else if (action == SLEEP)
		printf("\033[32m%zu %u is sleeping\033[0m\n", get_time(), nmb_philo);
	else if (action == THINK)
		printf("\033[34m%zu %u is thinking\033[0m\n", get_time(), nmb_philo);
	else if (action == DIE)
		printf("\033[31m%zu %u died\033[0m\n", get_time(), nmb_philo);
}

// Sleeps for the given time, but checks if the philosopher is still alive
void	monitored_sleep(t_philo *philo, unsigned int time)
{
	size_t	finish_time;

	finish_time = get_time() + time;
	while (get_time() < finish_time)
	{
		usleep(500);
		pthread_mutex_lock(philo->action_lock);
		if (!philo->is_living)
		{
			pthread_mutex_unlock(philo->action_lock);
			break ;
		}
		pthread_mutex_unlock(philo->action_lock);
	}
}
