/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:27:48 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/22 15:09:19 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(char *str, char chr)
{
	while (*str)
	{
		if (*str == chr)
			return (str);
		str++;
	}
	if (chr == '\0')
		return (str);
	return (NULL);
}

char	*ft_join_free(char *s1, char *s2)
{
	char	*result;
	size_t	i;
	size_t	j;

	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = 0;
	}
	i = ft_strchr(s1, '\0') - s1;
	result = malloc((i + (ft_strchr(s2, '\0') - s2) + 1) * sizeof(char));
	if (result)
	{
		i = -1;
		j = -1;
		while (s1[++i])
			result[i] = s1[i];
		while (s2[++j])
			result[i + j] = s2[j];
		result[i + j] = '\0';
	}
	free(s1);
	return (result);
}

void	ft_carriage(char *buffer, size_t amount)
{
	size_t	i;

	i = 0;
	if (amount == BUFFER_SIZE)
	{
		while (i < BUFFER_SIZE)
		{
			buffer[i] = '\0';
			i++;
		}
		return ;
	}
	while (buffer[amount + i])
	{
		buffer[i] = buffer[amount + i];
		i++;
	}
	while (i < BUFFER_SIZE)
	{
		buffer[i] = '\0';
		i++;
	}
}

char	*ft_handle_line(char *buffer, char *new_line)
{
	size_t	nli;

	new_line = ft_join_free(new_line, buffer);
	if (!new_line)
		return (NULL);
	if (!ft_strchr(new_line, '\n'))
	{
		ft_carriage(buffer, BUFFER_SIZE);
		return (new_line);
	}
	nli = ft_strchr(new_line, '\n') - new_line;
	new_line[nli + 1] = '\0';
	nli = ft_strchr(buffer, '\n') - buffer;
	ft_carriage(buffer, nli + 1);
	return (new_line);
}

char	*ft_main_loop(char *buffer, char *new_line, int fd, ssize_t *bytes)
{
	while (*bytes > 0)
	{
		*bytes = read(fd, buffer, BUFFER_SIZE);
		if (*bytes < 0)
		{
			if (new_line)
				free(new_line);
			ft_carriage(buffer, BUFFER_SIZE);
			return (NULL);
		}
		if (*bytes == 0)
			break ;
		buffer[*bytes] = '\0';
		if (ft_strchr(buffer, '\n'))
			return (ft_handle_line(buffer, new_line));
		new_line = ft_handle_line(buffer, new_line);
		if (!new_line)
		{
			ft_carriage(buffer, BUFFER_SIZE);
			return (NULL);
		}
	}
	return (new_line);
}
