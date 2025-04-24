/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:03:03 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/13 12:31:27 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	ft_subcount(char const *h, char n)
{
	int		i;
	size_t	c;

	i = 0;
	c = 0;
	if (n == 0)
	{
		if (*h)
			return (1);
		return (0);
	}
	while (h[i])
	{
		while (h[i] == n)
			i++;
		if (h[i])
			c++;
		while (h[i] != n && h[i])
			i++;
	}
	return (c);
}

static	size_t	ft_size(char const *h, char n)
{
	size_t	size;

	size = 0;
	while (h[size] != n && h[size])
		size++;
	return (size);
}

static void	ft_pop(char const *h, size_t *i, char n, char *split)
{
	size_t	j;

	j = 0;
	while (h[*i] != n && h[*i])
		split[j++] = h[(*i)++];
	split[j] = 0;
}

static char	**ft_depop(char **tab, int i)
{
	while (i--)
		free(tab[i]);
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char		**split;
	size_t		i;
	size_t		j;
	size_t		w;

	if (!s)
		return (NULL);
	w = ft_subcount(s, c);
	i = 0;
	j = 0;
	split = ft_calloc(w + 1, sizeof(char *));
	if (!split || !s)
		return (NULL);
	while (i < w)
	{
		while (s[j] == c && c)
			j++;
		split[i] = ft_calloc(ft_size(&s[j], c) + 1, sizeof(char));
		if (!split[i])
			return (ft_depop(split, i));
		ft_pop(s, &j, c, split[i++]);
	}
	split[i] = NULL;
	return (split);
}
