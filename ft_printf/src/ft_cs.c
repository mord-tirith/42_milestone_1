/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:30:25 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/28 13:26:46 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include <unistd.h>

void	ft_printf_c(int c, int *i)
{
	write(1, &c, 1);
	(*i)++;
}

void	ft_printf_s(char *s, int *i)
{
	size_t	j;

	if (!s)
		return (ft_printf_s("(null)", i));
	j = 0;
	while (s[j])
		j++;
	write(1, s, j);
	*i += j;
}
