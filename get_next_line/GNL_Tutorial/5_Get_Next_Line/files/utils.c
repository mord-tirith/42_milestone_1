#include "gnl.h"

/*
 	Sem comentários
 */ 

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

/*
 	Nosso coração da GNL, essa função adiciona os chars que estiverem em buffer à new_line
	Além disso, ela já dá free na new_line antiga, garantindo que nós temos manuseio perfeito
	de memória sem se preocupar com malabarismo de ponteiros
 */
char	*ft_join_free(char *line, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*big_line;

	if (!line)				// buffer nunca pode não existir, mas como line pode ser NULL, temos que garantir que ela existe
	{
		line = malloc(1);
		if (!line)
			return (NULL);
		line[0] = '\0';
	}

	big_line = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buffer) + 1)); // criamos a nova linha com espaço o bastante pra 
																				 // guardar a new_line antiga e o que houver no buffer
	
	if (big_line)
	{
		i = -1; // Criamos esses iteradores com -1 pra economizar 3 linhas em cada loop
		j = -1; // jogando o "++" dentro da condição do loop
		
		while (line[++i])					// primeiro copiamos da linha antiga para a nova...
			big_line[i] = line[i];
		
		while (buffer[++j])					// depois copiamos do buffer para a linha nova
			big_line[i + j] = buffer[j];
		
		big_line[i + j] = '\0';				// e metemos \0 no fim ^^
	}
	free(line);	// como prometido no título, damos free na linha original.
				// amizade encerrada com line: agora big_line é minha melhor amiga

	return (big_line);
}

/*
 	Função vital pra nossa GNL, ela retorna o index onde se encontra a primeira \n da string que recebe.
	Além disso, ela também retorna -1: algo essencial para podermos usar essa função tanto como uma
	verificação de "onde está a \n" quando de "há uma \n nessa string?"
 */

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

/*
 	Nossa controladora do buffer: essa função copia o que está depois da primeira \n no buffer
	pro começo dele, depois limpa tudo que resta pra nunca termos lixo em nosso buffer.
	Além disso, a função é esperta o bastante para limpar o buffer inteiro quando não há
	nenhuma \n dentro dele!
*/
void	ft_memmove_nl(char *buffer)
{
	ssize_t	i;		// Iterador simples
	ssize_t	nli;	// Iterador para a primeira \n no buffer

	i = 0;
	nli = ft_locate_nl(buffer) + 1; // nós precisamos limpar até a \n, ou seja, precisamos copiar de
									// 1 char a mais que o index de \n
	
	while (buffer[i + nli] && nli) // rodamos esse loop enquanto o iterador que começa na \n do buffer não chegar ao \0
	{
		buffer[i] = buffer[i + nli]; // copiamos à partir da \n pro começo do buffer
		i++;
	}
	while (i < BUFFER_SIZE) // agora é hora de limpar o que houver sobrado!
	{
		buffer[i] = '\0';   // um detalhe importante é que se ft_locate_nl(buffer) retornou -1, o valor de i nunca mudou de 0
		i++;				// ou seja, se não havia \n no buffer, vamos limpar ele inteiro
	}
}

/*
 	Talvez desnecessária, essa função só serve para dar uma limpeza e free perfeitos
	em buffer e new_line quando acontece um erro de leitura
 */

char	*ft_safe_free(char *line, char *buffer)
{
	size_t	i;

	if (line)		// um free seguro simples: se line não é NULL, damos free
		free(line);
	i = 0;
	while (i < BUFFER_SIZE) // e aqui uma limpeza garantida. buffer tem tamanho BUFFER_SIZE + 1
	{
		buffer[i] = '\0';	// ou seja, se limparmos ele do index 0 ao index BUFFER_SIZE, vamos 
							// ter apagado tudo que havia nele
		i++;
	}
	return (NULL);
}
