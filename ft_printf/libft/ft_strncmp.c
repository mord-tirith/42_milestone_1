/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:19:31 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/13 10:40:20 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strncmp(const char *str1, const char *str2, int num)
{
	int				i;

	if (num == 0)
		return (0);
	else if (num < 0)
		return (-1);
	i = 0;
	while (i < num)
	{
		if (str1[i] != str2[i])
			return ((unsigned char)str1[i] - (unsigned char)str2[i]);
		if (!str1[i])
			return (0);
		i++;
	}
	return (0);
}
