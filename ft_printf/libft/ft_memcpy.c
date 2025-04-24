/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:53:46 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 13:25:12 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest_str, const void *src_str, size_t n)
{
	size_t	i;

	if (!dest_str && !src_str)
		return (dest_str);
	i = 0;
	if (n > 0)
	{
		while (n > i)
		{
			((unsigned char *)dest_str)[i] = ((unsigned char *)src_str)[i];
			i++;
		}
	}
	return (dest_str);
}
