#include "../lib/gnl_tutorial.h"
#include "ft_join_free.c"

#define INVALID_IN "Pfvr executar com um desses argumentos:\n 1 - lyrics.txt\n2 - long_lorem.txt\n3 - seu próprio arquivo!\n"


/*
	Como disse no tutorial, eu PESSOALMENTE prefiro usar ft_strchr para buscar as \n. Porém,
	caso queira criar uma função para isso, essa é minha recomendação: uma simples função que
	retorna o index da \n na string, ou um -1 se a string não contém nenhum \n
*/
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
	char	*buffer;	 // Alvo da nossa read()
	char	*line;  	 // Linha que estamos criando para o usuário
	ssize_t	read_result; // Quantidade de bytes que a read leu

	line = NULL;
	buffer = calloc(BUFFER_SIZE + 1, sizeof(char)); // Iniciamos nosso buffer
	if (!buffer)
		return (NULL);
	read_result = 1; // Como o loop a seguir depende de read_result para funcionar, iniciamos ela com valor 1 para entrarmos no loop pela primeira vez! Depois disso, o valor de read_result vai depender da função read.
	while (read_result)
	{
		read_result = read(fd, buffer, BUFFER_SIZE); // Lemos BUFFER_SIZE bytes pro nosso buffer
		if (read_result < 0) // Se a leitura deu errado, limpamos os mallocs e saímos!
		{
			free(buffer);
			if (line)
				free(line);
			return (NULL);
		}
		if (read_result) // se lemos sequer 1 byte...
			line = ft_join_free(line, buffer); // temos coisa pra adicionar na linha!
		if (ft_nl_finder(buffer) >= 0) // Se acharmos um \n no buffer, é hora de parar de ler!
			break ;
	}
	if (ft_nl_finder(line) >= 0) // Botamos um \0 um caracter depois do \n pra encerrar a linha
		line[ft_nl_finder(line) + 1] = 0;
	free(buffer);
	return (line);
}

/*
	Este programa é bem mais coplexo que o get, dê tempo para entender tudo nele!
	Teste ver se o programa ainda retorna apenas a primeira linha de um arquivo com diferentes BUFFER_SIZEs.
	Teste arquivos não existentes, e teste botar um arquivo seu no lugar da variável "personal"!
	Repare o estranho erro que ocorre se você mandar esse programa ler um arquivo inteiro que não contém uma única \n: consegue adivinhar o problema?
*/

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
