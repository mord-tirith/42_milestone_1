/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:40:28 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 09:17:58 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	findsize(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	put_ip(long n, char *s, int size)
{
	int	i;

	i = size - 1;
	while (i >= 0)
	{
		s[i] = (n % 10) + 48;
		n /= 10;
		i--;
	}
	s[size] = 0;
}

static void	put_in(long n, char *s, int size)
{
	int	i;

	i = size - 1;
	while (i > 0)
	{
		s[i] = (n % 10) + 48;
		n /= 10;
		i--;
	}
	s[size] = 0;
	s[0] = '-';
}

char	*ft_itoa(int number)
{
	long	n;
	char	*buff;
	int		size;

	n = (long)number * ((number > 0) - (number < 0));
	size = findsize(n);
	if (number < 0)
		buff = ft_calloc(size + 2, sizeof(char));
	else
		buff = ft_calloc(size + 1, sizeof(char));
	if (!buff)
		return (NULL);
	if (number < 0)
		put_in(n, buff, size + 1);
	else
		put_ip(n, buff, size);
	return (buff);
}
