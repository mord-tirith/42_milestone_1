/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:44:28 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/24 08:21:36 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdint.h>

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*arr;
	size_t	num_bytes;

	if (nitems == 0 || size == 0)
		return (malloc(0));
	if (nitems > SIZE_MAX / size)
		return (NULL);
	num_bytes = nitems * size;
	arr = malloc(num_bytes);
	if (!arr)
		return (NULL);
	ft_bzero(arr, num_bytes);
	return (arr);
}
