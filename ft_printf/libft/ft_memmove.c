/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:51:49 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 09:33:02 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_movebigger(char *d, const char *s, size_t n)
{
	while (n)
	{
		*d = *s;
		s++;
		d++;
		n--;
	}
}

static void	ft_movesmall(char *d, const char *s, size_t n)
{
	while (n)
	{
		n--;
		d[n] = s[n];
	}
}

void	*ft_memmove(void *dest_str, const void *src_str, size_t n)
{
	char		*d;
	const char	*s;

	s = src_str;
	d = dest_str;
	if (s == d || !n)
		return (d);
	if (s > d)
		ft_movebigger(d, s, n);
	else
		ft_movesmall(d, s, n);
	return (d);
}
