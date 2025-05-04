#include "gnl.h"

static char *ft_handle_nl(char *buffer, char *new_line)
{
    int nl_index;

    nl_index = ft_locate_nl(new_line);
    if (nl_index >= 0)
        new_line[nl_index + 1] = '\0';
    ft_memmove_nl(buffer);
    return (new_line);
}

static char    *ft_read_loop(char *buffer, char *new_line, int fd)
{
    ssize_t bytes_read;

	ft_memmove_nl(buffer);
    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0)
            return (ft_safe_free(new_line, buffer));
        if (bytes_read == 0)
			break ;
		new_line = ft_join_free(new_line, buffer);
        if (!new_line)
            return (NULL);
        if (ft_locate_nl(buffer) >= 0)
            break ;
		ft_memmove_nl(buffer);
    }
	return (new_line);
}

char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line = NULL;
    if (buffer[0] != '\0')
        new_line = ft_join_free(new_line, buffer);
    if (ft_locate_nl(buffer) >= 0)
        return (ft_handle_nl(buffer, new_line));
    new_line = ft_read_loop(buffer, new_line, fd);
    new_line = ft_handle_nl(buffer, new_line);
    return (new_line);
}
