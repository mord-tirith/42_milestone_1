/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:17:35 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/28 13:27:13 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_parsel(char spec, va_list arg, int *i)
{
	if (spec == '%')
		ft_printf_c('%', i);
	if (spec == 'c')
		ft_printf_c(va_arg(arg, int), i);
	if (spec == 's')
		ft_printf_s(va_arg(arg, char *), i);
	if (spec == 'd' || spec == 'i')
		ft_printf_di(va_arg(arg, int), i);
	if (spec == 'u')
		ft_printf_u(va_arg(arg, unsigned int), i);
	if (spec == 'x')
		ft_printf_x(va_arg(arg, unsigned int), i, 'l');
	if (spec == 'X')
		ft_printf_x(va_arg(arg, unsigned int), i, 'u');
	if (spec == 'p')
		ft_printf_p(va_arg(arg, void *), i);
}

static int	ft_validate(char c)
{
	char	*valid_input;

	valid_input = "cspdiuxX%";
	while (*valid_input)
	{
		if (c == *valid_input)
			return (1);
		valid_input++;
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && ft_validate(*(format + 1)))
		{
			format++;
			ft_parsel(*format, args, &count);
			if (*format == '&')
				format++;
		}
		else
			ft_printf_c(*format, &count);
		format++;
	}
	va_end(args);
	return (count);
}
