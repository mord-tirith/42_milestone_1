#include "../lib/gnl_tutorial.h"

/*
	Uma simples strlen, nada demais aqui:
*/
static size_t	ft_strl(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_join_free(char *line, char *buffer)
{
	char	*bigger_line; // Nova linha que será criada
	size_t	i;			  // Iterador para a string line
	size_t	j;			  // Iterador para a string buffer

	// Nossa função SEMPRE dá free em line. Assim sendo, temos que
	// garantir que line não é nula antes de começarmos!
	if (!line)
	{
		line = malloc(1);
		if (!line)
			return (NULL);
		line[0] = 0;
	}
	// A nova linha (ou "linha maior" nesse caso) precisa ser grande o bastante
	// para guardar tudo que já havia na line E o que houver na buffer!
	bigger_line = malloc(ft_strl(line) + ft_strl(buffer) + 1 * sizeof(char));
	if (bigger_line)
	{
		i = -1;	// Se n conhece esse truque, é uma ótima forma de reduzir um loop de iteração
		j = -1;	// para apenas 1 linha de código, salvando espaço na norminette ;)
		while (line[++i])
			bigger_line[i] = line[i];
		while (buffer[++j])
			bigger_line[i + j] = buffer[j];
		bigger_line[i + j] = 0;
	}
	free(line); // Damos free no ponteiro de line antigo, pra não termos memory leak!
	return (bigger_line);
}
