#include "get_next_line_bonus.h"
#include <stdio.h>
#include <fcntl.h>

int main()
{
	int	fd1 = open("odds.txt", O_RDONLY);
	int	fd2 = open("evens.txt", O_RDONLY);

	char	*evens = get_next_line(fd2);
	char	*odds = get_next_line(fd1);
	if (!evens || !odds)
	{
		if (evens)
			free(evens);
		if (odds)
			free(odds);
		return (0);
	}
	while (evens || odds)
	{
		if (odds)
		{
			printf("%s", odds);
			free(odds);
		}
		if (evens)
		{
			printf("%s", evens);
			free(evens);
		}
		evens = get_next_line(fd2);
		odds = get_next_line(fd1);
	}
	close(fd1);
	close(fd2);
	return (0);
}
