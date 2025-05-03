/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thenriqu <thenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:14:47 by thenriqu          #+#    #+#             */
/*   Updated: 2025/05/03 12:43:09 by thenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/gnl_tutorial.h"


/*
	Para começo de conversa, uma versão infinitamente simplista do GNL:
	esta função apenas cria um malloc com ponteiro grande o bastante para guardar "BUFFER_SIZE + 1"
	caractéres, depois usa a função read() para jogar BUFFER_SIZE caractéres do fd para dentro
	desse endereço de memória, bota um nulo no final e retorna a linha.
*/
char	*get_1(int fd, ssize_t *bytes_read)
{
	char	*first_line;
	
	first_line = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (first_line)
	{
		*bytes_read += read(fd, first_line, BUFFER_SIZE);
		first_line[bytes_read] = 0;
	}
	return (first_line);
}

/*
	Nada demais para se ver nessa main, você escolhe quantas chamadas repetidas da função get_1
	você quer, e a main faz o trabalho pra você. Se quer ver algo interessante, tente compilar
	Este programa com diferentes BUFFER_SIZEs, algo como
	cc get.c -D BUFFER_SIZE=1000
	Ou
	cc get.c -D BUFFER_SIZE=1
	E veja como ela se comporta!
*/
int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	ssize_t	bytes;

	bytes = 0;
	if (argc != 2)
		return (printf("Pfvr usar uma das opções a seguir:\n1 - get 1x\n2 - get 2x\n3 - get 3x\n"), 0);
	fd = open("lyrics.txt", O_RDONLY);
	if (argv[1][0] == '1' || argv[1][0] == '2' || argv[1][0] == '3')
	{
		line = get_1(fd, &bytes);
		printf("%s", line);
		free(line);
	}
	if (argv[1][0] == '2' || argv[1][0] == '3')
	{
		line = get_1(fd, &bytes);
		printf("%s", line);
		free(line);
	}
	if (argv[1][0] == '3')
	{
		line = get_1(fd, &bytes);
		printf("%s", line);
		free(line);
	}
	printf("\nBytes lidos ao todo:%zd\nTamanho de BUFFER_SIZE:%d\n", bytes, BUFFER_SIZE);
	close(fd);
	return 0;
}

/*
	Ps: se achar uma forma de fazer esse programa dar memory leak, favor contactar thenriqu no slack
	e me avisar <3
*/
