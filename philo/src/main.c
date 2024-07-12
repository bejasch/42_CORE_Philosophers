/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:15:46 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/10 15:28:00 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

static char	handle_inputs(int argc, char **argv, t_infos *info);

// 1. number_of_philosophers
// 2. time_to_die
// 3. time_to_eat
// 4. time_to_sleep
// 5. [number_of_times_each_philosopher_must_eat]
int	main(int argc, char **argv)
{
	t_infos			infos;
	unsigned int	id;

	if (!handle_inputs(argc, argv, &infos))
		return (err_input(), 1);
	if (!init_locks(&infos) || !init_philos(&infos))
		return (destroy_infos(&infos), 1);
	die_check(&infos, 1);
	id = infos.nmb_philo;
	while (id--)
		if (pthread_join(infos.philo_thr[id], NULL))
			return (destroy_infos(&infos), 1);
	destroy_infos(&infos);
	return (0);
}

// checks for valid inputs and sets the info-struct with it
static char	handle_inputs(int argc, char **argv, t_infos *info)
{
	if (argc < 5 || argc > 6)
		return (0);
	memset(info, 0, sizeof(t_infos));
	if (!ft_atoui(argv[1], &info->nmb_philo)
		|| !ft_atoui(argv[2], &info->die_time)
		|| !ft_atoui(argv[3], &info->eat_time)
		|| !ft_atoui(argv[4], &info->sleep_time))
		return (0);
	info->nmb_eats = 0;
	if (argc == 6 && !ft_atoui(argv[5], &info->nmb_eats))
		return (0);
	printf("Infos: \n # of philosophers: %d\n time to die:    %8d ms\n eating \
time:    %8d ms\n sleeping time:  %8d ms \n number of eats: %d\n", 
		info->nmb_philo, info->die_time, info->eat_time, info->sleep_time, \
		info->nmb_eats);
	return (1);
}
