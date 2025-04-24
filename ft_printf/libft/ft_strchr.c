/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:52:02 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 09:23:34 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	char			*s;
	unsigned char	search;

	search = c;
	s = (char *)str;
	while (*s)
	{
		if (*s == search)
			return (s);
		s++;
	}
	if (search == 0)
		return (s);
	return (NULL);
}
