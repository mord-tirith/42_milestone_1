/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:05:49 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/24 10:03:49 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

// Includes
# include "libft/libft.h"
# include <stdarg.h>

// Prototypes
/// Main
int	ft_printf(char *format, ...);
/// Helpers
void	ft_printf_c(int c, int *i);
void	ft_printf_s(char *s, int *i);
void	ft_printf_di(int n, int *i);
void	ft_printf_u(unsigned int n, int *i);
void	ft_printf_x(unsigned int n, int *i, char mode);
void	ft_printf_p(void *ptr, int *i);

#endif
