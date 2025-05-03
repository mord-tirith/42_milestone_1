#include "../lib/gnl_tutorial.h"


int	count_local()
{
	int	i = 0;

	i++;
	return (i);
}

int	count_static()
{
	static int i = 0;	// <- Comente isso, descomente isso \/ e experimente!
	//static int i;		// <- Comente isso, descomente isso /\ e experimente!
	i++;
	return (i);
}

int	main()
{
	int	i = 0;
	int	j = 0;

	while (i < 100 && j < 100)
	{
		i = count_local();
		j = count_static();
		printf("Local: %d Static: %d|| ", i, j);
		if (j % 10 == 0 || i % 10 == 0)
			printf("\n");
	}
	return 0;
}
