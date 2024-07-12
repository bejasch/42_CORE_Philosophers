/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 19:48:48 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 12:54:30 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

// Prints an error message depending on the error code
void	error_msg(t_action opt)
{
	if (opt == INPUT_ERR)
		write(1, "Wrong inputs!\n", 15);
	else if (opt == FORK_ERR)
		write(1, "Fork error\n", 11);
	else if (opt == PHILO_ERR)
		write(1, "Philo creation failed\n", 16);
	else if (opt == SEM_NAME_ERR)
		write(1, "Semaphore name error\n", 21);
	else if (opt == MALLOC_ERR)
		write(1, "Malloc error\n", 21);
	else if (opt == SEM_OPEN_ERR)
		write(1, "Error opening a semaphore\n", 16);
}

// Kills all philosophers processes and eat_cycle process
void	kill_all_childpr(t_infos *infos, pid_t pid)
{
	unsigned int	id;

	id = infos->nmb_philo;
	while (id--)
		kill(infos->philos[id], SIGKILL);
	kill(pid, SIGKILL);
}

// Transforms a string to an unsigned int (result); returns 1 on success
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
void	printing_philostate(t_infos *infos, t_action action)
{
	unsigned int	nmb_philo;

	nmb_philo = infos->philo_id + 1;
	sem_wait(infos->print_sem);
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
	sem_post(infos->print_sem);
}
