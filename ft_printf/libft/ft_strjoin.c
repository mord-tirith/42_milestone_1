/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 09:56:10 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/13 12:30:32 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	size;
	char	*joint;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2);
	joint = ft_calloc(size + 1, sizeof(char));
	if (!joint)
		return (NULL);
	joint[0] = 0;
	ft_strlcpy(joint, s1, size + 1);
	ft_strlcat(joint, s2, size + 1);
	return (joint);
}
