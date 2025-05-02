#include <stdio.h>

void	change_array(char *old, char *new)
{
	int	i;

	i = 0;
	while (old[i])
	{
		old[i] = new[i];
		i++;
	}
}

int main()
{
	char a[50] = "abcdefghijkl";

	change_array(a, "hello world!\0");
	printf("%s\n", a);
}
