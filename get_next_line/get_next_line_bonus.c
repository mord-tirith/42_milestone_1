/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 07:27:56 by thenriqu          #+#    #+#             */
/*   Updated: 2025/04/22 16:20:23 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	buffer[FD_MAX][BUFFER_SIZE + 1];
	char		*line;
	ssize_t		bytes;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || ((read(fd, 0, 0) < 0) && !buffer[fd][0]))
		return (NULL);
	if (read(fd, 0, 0) < 0 && buffer[fd][0])
	{
		ft_carriage(buffer[fd], BUFFER_SIZE);
		return (NULL);
	}
	bytes = 1;
	if (buffer[fd][0] && ft_strchr(buffer[fd], '\n'))
		return (ft_handle_line(buffer[fd], line));
	else if (buffer[fd][0])
		line = ft_handle_line(buffer[fd], line);
	line = ft_main_loop(buffer[fd], line, fd, &bytes);
	if (!line && (!buffer[fd][0] || bytes < 0))
	{
		ft_carriage(buffer[fd], BUFFER_SIZE);
		return (NULL);
	}
	return (line);
}
