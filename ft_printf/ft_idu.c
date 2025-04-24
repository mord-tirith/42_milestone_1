/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_idu.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:25:16 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/24 09:43:22 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_digicount(unsigned int n)
{
	int	count;

	count = 0;
	if (!n)
		return (1);
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

void	ft_printf_di(int n, int *i)
{
	char			number[12];
	unsigned int	num;
	int				pos;

	pos = ft_digicount((n * ((n > 0) - (n < 0))));
	if (n < 0)
	{
		pos++;
		number[0] = '-';
		num = -n;
	}
	else
		num = n;
	number[pos] = '\0';
	while (num)
	{
		number[pos - 1] = (num % 10) + '0';
		num /= 10;
		pos--;
	}
	if (!n)
		number[0] = '0';
	ft_printf_s(number, i);
}

void	ft_printf_u(unsigned int n, int *i)
{
	char	number[11];
	int		pos;

	if (!n)
	{
		ft_printf_c('0', i);
		return ;
	}
	pos = ft_digicount(n);
	number[pos] = '\0';
	while (n)
	{
		number[pos - 1] = (n % 10) + '0';
		pos--;
		n /= 10;
	}
	ft_printf_s(number, i);
}
