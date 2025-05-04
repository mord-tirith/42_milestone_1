#include "gnl.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_join_free(char *line, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*big_line;

	if (!line)
	{
		line = malloc(1);
		if (!line)
			return (NULL);
		line[0] = '\0';
	}
	big_line = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buffer) + 1));
	if (big_line)
	{
		i = -1;
		j = -1;
		while (line[++i])
			big_line[i] = line[i];
		while (buffer[++j])
			big_line[i + j] = buffer[j];
		big_line[i + j] = '\0';
	}
	free(line);
	return (big_line);
}

ssize_t	ft_locate_nl(char *str)
{
	ssize_t	i;

	if (!str)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void	ft_memmove_nl(char *str)
{
	ssize_t	i;
	ssize_t	j;

	if (!str)
		return ;
	i = 0;
	j = ft_locate_nl(str) + 1;
	while (str[i + j] && j)
	{
		str[i] = str[i + j];
		i++;
	}
	while (i < BUFFER_SIZE)
	{
		str[i] = '\0';
		i++;
	}
}

char	*ft_safe_free(char *line, char *buffer)
{
	ssize_t	i;

	if (line)
		free(line);
	i = ft_locate_nl(buffer);
	while (i != -1)
	{
		buffer[i] = 42;
		i = ft_locate_nl(buffer);
	}
	ft_memmove_nl(buffer);
	return (NULL);
}
