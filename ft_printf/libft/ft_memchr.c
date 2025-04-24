/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:58:04 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 10:32:19 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	needle;
	unsigned char	*str;
	size_t			i;

	i = 0;
	needle = c;
	str = (unsigned char *)s;
	while (i < n)
	{
		if (str[i] == needle)
			return (&str[i]);
		i++;
	}
	return (NULL);
}
