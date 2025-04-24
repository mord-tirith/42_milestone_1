/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 09:53:31 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/12 10:41:03 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	setmem;
	unsigned char	*ret;
	size_t			i;

	ret = (unsigned char *)str;
	i = 0;
	setmem = c;
	while (i < n)
		ret[i++] = setmem;
	return (ret);
}
