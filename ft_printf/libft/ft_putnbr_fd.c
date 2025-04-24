/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:41:15 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 11:13:58 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_length(unsigned int n)
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

static void	ft_rec(char *buffer, int i, unsigned int n)
{
	if (n >= 10)
		ft_rec(buffer, i - 1, n / 10);
	buffer[i] = (n % 10) + '0';
}

void	ft_putnbr_fd(int n, int fd)
{
	char			buff[11];
	unsigned int	num;

	ft_bzero(buff, 11);
	if (n < 0)
		ft_putchar_fd('-', fd);
	num = n * ((n > 0) - (n < 0));
	ft_rec(buff, ft_length(num) - 1, num);
	ft_putstr_fd(buff, fd);
}
