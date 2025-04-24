/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:00:35 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/14 15:36:11 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static	int	ft_inset(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static void	ft_findp(const char *s1, const char *set, int *i)
{
	i[0] = 0;
	i[1] = ft_strlen(s1) - 1;
	while (s1[i[0]] && ft_inset(s1[i[0]], set))
		i[0]++;
	while (i[1] >= 0 && ft_inset(s1[i[1]], set))
		i[1]--;
}

static char	*ft_returnempty(void)
{
	char	*faux;

	faux = ft_calloc(1, sizeof(char));
	if (!faux)
		return (NULL);
	faux[0] = 0;
	return (faux);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		i[2];
	int		j;
	int		size;
	char	*trim;

	if (!s1 || !set)
		return (NULL);
	j = 0;
	ft_findp(s1, set, i);
	size = i[1] - i[0] + 1;
	if (size < 0)
		return (ft_returnempty());
	trim = ft_calloc(size + 1, sizeof(char));
	if (!trim)
		return (NULL);
	while (i[0] <= i[1])
	{
		trim[j] = s1[i[0]];
		i[0]++;
		j++;
	}
	trim[j] = 0;
	return (trim);
}
