/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:42:24 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/11 11:03:43 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

static void	eat_cycle(t_infos *infos);
static void	dummy_routine(void);
static char	start_processes(t_infos *infos, unsigned int id);

// Creates and stops the philosophers, eat and dummy processes
void	create_philos(t_infos *infos)
{
	pid_t			pid;
	unsigned int	id;
	int				status;

	pid = fork();
	if (pid == -1)
		return (error_msg(FORK_ERR));
	else if (!pid)
		return (eat_cycle(infos));
	if (!start_processes(infos, infos->nmb_philo))
		return ;
	id = infos->nmb_philo;
	while (id--)
		waitpid(infos->dummys[id], &status, 0);
	kill_all_childpr(infos, pid);
	waitpid(-1, &status, 0);
}

// Gives the philosophers the ability to eat in a cycle
static void	eat_cycle(t_infos *infos)
{
	unsigned int	id;

	id = infos->nmb_philo;
	while (id--)
		sem_wait(infos->action_locks[id]);
	while (1)
	{
		id = infos->nmb_philo;
		while (id--)
		{
			sem_post(infos->action_locks[id]);
			usleep(100);
			sem_wait(infos->action_locks[id]);
		}
	}
}

// Dummy process, that does nothing but waits for a signal to die
static void	dummy_routine(void)
{
	while (1)
		usleep(999999);
}

// Starts dummys and philosophers processes
static char	start_processes(t_infos *infos, unsigned int id)
{
	while (id--)
	{
		infos->dummys[id] = fork();
		if (infos->dummys[id] == -1)
			return (error_msg(FORK_ERR), 0);
		else if (!infos->dummys[id])
			return (dummy_routine(), 0);
	}
	id = infos->nmb_philo;
	while (id--)
	{
		infos->philos[id] = fork();
		if (infos->philos[id] == -1)
			return (error_msg(FORK_ERR), 0);
		else if (!infos->philos[id])
			return (philo_routine(id, infos), 0);
	}
	return (1);
}
