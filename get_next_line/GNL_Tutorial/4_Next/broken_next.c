#include "../lib/gnl_tutorial.h"
#include "../3_Line/ft_join_free.c"

int		ft_nl_finder(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_next(int fd)
{
	char	*buffer;
	char	*line;
	ssize_t	read_result;

	line = NULL;
	buffer = calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	read_result = 1;
	while (read_result)
	{
		read_result = read(fd, buffer, BUFFER_SIZE);
		if (read_result < 0)
		{
			free(buffer);
			if (line)
				free(line);
			return (NULL);
		}
		if (read_result)
			line = ft_join_free(line, buffer);
		if (ft_nl_finder(buffer) >= 0)
			break ;
	}
	if (ft_nl_finder(line) >= 0)
		line[ft_nl_finder(line) + 1] = 0;
	free(buffer);
	return (line);
}

int	main()
{
	int	fd = open("lyrics.txt", O_RDONLY);
	char *a = get_next(fd);
	printf("%s", a);
	free(a);
	a = get_next(fd);
	printf("%s", a);
	free(a);
	close(fd);
	return (0);
}
