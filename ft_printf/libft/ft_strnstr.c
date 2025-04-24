/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 20:40:10 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/13 12:39:13 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *hs, const char *nd, size_t size)
{
	size_t		n_size;
	size_t		i;
	char		*h;

	if (!hs && !size)
		return (NULL);
	h = (char *)hs;
	i = 0;
	n_size = ft_strlen(nd);
	if (!*nd)
		return (h);
	while (h[i] && i + n_size <= size)
	{
		if (!ft_strncmp(&h[i], nd, n_size))
			return (&h[i]);
		i++;
	}
	return (NULL);
}
