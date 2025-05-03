#include "../lib/gnl_tutorial.h"
#include "ft_join_free.c"

#define INVALID_IN "Pfvr executar com um desses argumentos:\n 1 - lyrics.txt\n2 - long_lorem.txt\n3 - seu próprio arquivo!\n"

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

char	*get_line(int fd)
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
		if (read_result == 0)
			break ;
		line = ft_join_free(line, buffer);
		if (ft_nl_finder(buffer) >= 0)
			break ;
	}
	if (ft_nl_finder(line) >= 0)
		line[ft_nl_finder(line) + 1] = 0;
	free(buffer);
	return (line);
}

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	char	*lyrics = "lyrics.txt";
	char	*long_line = "long_lorem.txt";
	char	*personal = "seu_arquivo_aqui.txt";

	if (argc != 2)
		return (printf("%s", INVALID_IN), 0);
	if (argv[1][0] == '1')
		fd = open(lyrics, O_RDONLY);
	else if (argv[1][0] == '2')
		fd = open(long_line, O_RDONLY);
	else
		fd = open(personal, O_RDONLY);
	line = get_line(fd);
	printf("%s", line);
	free(line);
	close(fd);
	return (0);
}
