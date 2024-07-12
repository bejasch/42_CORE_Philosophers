/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:03:20 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/10 20:01:47 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

// 1. number_of_philosophers
// 2. time_to_die
// 3. time_to_eat
// 4. time_to_sleep
// 5. [number_of_times_each_philosopher_must_eat]
int	main(int argc, char **argv)
{
	t_infos		infos;

	if (!handle_inputs(argc, argv, &infos))
		return (error_msg(INPUT_ERR), 1);
	if (!init_forks_philos(&infos))
		return (destroy_infos(&infos), 1);
	create_philos(&infos);
	destroy_infos(&infos);
	return (0);
}
