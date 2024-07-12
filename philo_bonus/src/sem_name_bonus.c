/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_name_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 21:58:12 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/10 20:03:10 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

static int	uintlen(unsigned int n);
static void	uintwrite(unsigned int n, char *write, int len);

// Allocates (with malloc(3)) and returns a string representing a semaphor
// name in form of "/act_n_" (if opt) or "/die_n_" with argument _n_ .
char	*ito_semname(unsigned int n, char opt)
{
	int		len;
	char	*out;

	if (!n)
		len = 1;
	else
		len = uintlen(n);
	out = (char *)malloc((len + 4 + 1) * sizeof(char));
	if (!out)
		return (error_msg(MALLOC_ERR), NULL);
	out[0] = '/';
	if (opt)
	{
		out[1] = 'a';
		out[2] = 'c';
		out[3] = 't';
	}
	else
	{
		out[1] = 'd';
		out[2] = 'i';
		out[3] = 'e';
	}
	uintwrite(n, out + 4, len);
	return (out);
}

static int	uintlen(unsigned int n)
{
	if (!n)
		return (0);
	return (1 + uintlen(n / 10));
}

static void	uintwrite(unsigned int n, char *write, int len)
{
	unsigned int	value;
	int				i;

	if (!len)
		*write = '\0';
	else
	{
		value = n;
		i = 1;
		while (i++ < len)
			value /= 10;
		*write = '0' + value % 10;
		uintwrite(n, write + 1, len - 1);
	}
}
