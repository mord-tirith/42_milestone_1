/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:27:56 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/28 10:56:05 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		bytes;

	line = NULL;
	if (fd < 0)
		return (NULL);
	bytes = 1;
	if (buffer[0] && ft_strchr(buffer, '\n'))
		return (ft_handle_line(buffer, line));
	else if (buffer[0])
		line = ft_handle_line(buffer, line);
	line = ft_main_loop(buffer, line, fd, &bytes);
	if (!line && (!buffer[0] || bytes < 0))
	{
		ft_carriage(buffer, BUFFER_SIZE);
		return (NULL);
	}
	return (line);
}
