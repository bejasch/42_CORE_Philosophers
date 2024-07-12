/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_free_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:17:29 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/11 10:51:57 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

static char	init_semaphores(t_infos *infos);
static void	close_unlink_sem(t_infos *infos, unsigned int id);

// checks for valid inputs and sets the info-struct with it
char	handle_inputs(int argc, char **argv, t_infos *infos)
{
	if (argc < 5 || argc > 6)
		return (0);
	memset(infos, 0, sizeof(t_infos));
	if (!ft_atoui(argv[1], &infos->nmb_philo)
		|| !ft_atoui(argv[2], &infos->die_time)
		|| !ft_atoui(argv[3], &infos->eat_time)
		|| !ft_atoui(argv[4], &infos->sleep_time))
		return (0);
	if (argc == 6 && !ft_atoui(argv[5], &infos->nmb_eats))
		return (0);
	infos->is_living = 1;
	printf("Infos: \n # of philosophers: %d\n time to die:    %8d ms\n eating \
time:    %8d ms\n sleeping time:  %8d ms \n number of eats: %d\n", 
		infos->nmb_philo, infos->die_time, infos->eat_time, infos->sleep_time, \
		infos->nmb_eats);
	return (1);
}

// inits forks as a semaphore with # of philosophers as initial value
char	init_forks_philos(t_infos *infos)
{
	infos->action_locks = malloc(sizeof(sem_t *) * infos->nmb_philo);
	if (!infos->action_locks)
		return (error_msg(MALLOC_ERR), 0);
	memset(infos->action_locks, 0, sizeof(sem_t *) * infos->nmb_philo);
	infos->die_locks = malloc(sizeof(sem_t *) * infos->nmb_philo);
	if (!infos->die_locks)
		return (error_msg(MALLOC_ERR), 0);
	memset(infos->die_locks, 0, sizeof(sem_t *) * infos->nmb_philo);
	infos->philos = malloc(sizeof(pid_t) * infos->nmb_philo);
	if (!infos->philos)
		return (error_msg(MALLOC_ERR), 0);
	memset(infos->philos, 0, sizeof(pid_t) * infos->nmb_philo);
	infos->dummys = malloc(sizeof(pid_t) * infos->nmb_philo);
	if (!infos->dummys)
		return (error_msg(MALLOC_ERR), 0);
	memset(infos->dummys, 0, sizeof(pid_t) * infos->nmb_philo);
	sem_unlink("/forks_sem");
	infos->forks_sem = sem_open("/forks_sem", O_CREAT, 0600, infos->nmb_philo);
	if (infos->forks_sem == SEM_FAILED)
		return (error_msg(SEM_OPEN_ERR), 0);
	sem_unlink("/print_sem");
	infos->print_sem = sem_open("/print_sem", O_CREAT, 0600, 1);
	if (infos->forks_sem == SEM_FAILED)
		return (error_msg(SEM_OPEN_ERR), 0);
	return (init_semaphores(infos));
}

// Inits the die- and action-semaphores for the philosophers with names
static char	init_semaphores(t_infos *infos)
{
	unsigned int	id;
	char			*sem_name;

	id = infos->nmb_philo;
	while (id--)
	{
		sem_name = ito_semname(id, 1);
		if (!sem_name)
			return (error_msg(SEM_NAME_ERR), 0);
		sem_unlink(sem_name);
		infos->action_locks[id] = sem_open(sem_name, O_CREAT, 0600, 1);
		if (infos->action_locks[id] == SEM_FAILED)
			return (free(sem_name), error_msg(SEM_OPEN_ERR), 0);
		free(sem_name);
		sem_name = ito_semname(id, 0);
		if (!sem_name)
			return (error_msg(SEM_NAME_ERR), 0);
		sem_unlink(sem_name);
		infos->die_locks[id] = sem_open(sem_name, O_CREAT, 0600, 1);
		if (infos->die_locks[id] == SEM_FAILED)
			return (free(sem_name), error_msg(SEM_OPEN_ERR), 0);
		free(sem_name);
	}
	return (1);
}

// checks and frees all allocated memory
void	destroy_infos(t_infos *infos)
{
	close_unlink_sem(infos, infos->nmb_philo);
	if (infos->action_locks)
		free (infos->action_locks);
	if (infos->die_locks)
		free (infos->die_locks);
	if (infos->philos)
		free (infos->philos);
	if (infos->dummys)
		free (infos->dummys);
}

static void	close_unlink_sem(t_infos *infos, unsigned int id)
{
	char			*sem_name;

	if (infos->forks_sem)
		sem_close(infos->forks_sem);
	sem_unlink("/forks_sem");
	if (infos->print_sem)
		sem_close(infos->print_sem);
	sem_unlink("/print_sem");
	while (id--)
	{
		sem_name = ito_semname(id, 1);
		if (!sem_name)
			return (error_msg(SEM_NAME_ERR));
		if (infos->action_locks[id])
			sem_close(infos->action_locks[id]);
		sem_unlink(sem_name);
		free(sem_name);
		sem_name = ito_semname(id, 0);
		if (!sem_name)
			return (error_msg(SEM_NAME_ERR));
		if (infos->die_locks[id])
			sem_close(infos->die_locks[id]);
		sem_unlink(sem_name);
		free(sem_name);
	}
}
