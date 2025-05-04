#include "gnl.h"


/*
 	Essa função é responsável por devolver apenas o texto até a \n pro usuário, além de
	limpar o buffer para deixá-lo pronto para a próxima vez que GNL for chamada
 */
static char *ft_handle_nl(char *buffer, char *new_line)
{
    int nl_index;

	nl_index = ft_locate_nl(new_line); // Buscamos em que index de new_line se encontra o \n
	if (nl_index >= 0)
		new_line[nl_index + 1] = '\0'; // Botamos um \0 um char depois da \n, cortando a new_line
	ft_memmove_nl(buffer); // Chamamos nossa helper que lida com o buffer
	return (new_line);
}

/*
 	Nosso loop de leitura, que está aqui porquê a Norminette é uma bitch
 */
static char    *ft_read_loop(char *buffer, char *new_line, int fd)
{
    ssize_t bytes_read;

	ft_memmove_nl(buffer); // Para evitar lixo deixado no buffer em rodadas anteriores, limpamos ele antes de ler.
						   // Isso não é problemático porquê nós nunca chegamos nessa função sem ter antes passado
						   // o que estivesse no buffer para a new_line: podemos limpar ele sem medo!

    while (1) // Todas condições pra terminar o loop dependem do que ocorre em bytes_read, portanto podemos fazer um loop "eterno"
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE); // lemos do fd para nosso buffer
        if (bytes_read < 0)
            return (ft_safe_free(new_line, buffer)); // limpamos tudo e retornamos NULL se a leitura deu errado
        if (bytes_read == 0) // saímos do loop se tentamos ler mas já estávamos no fim do fd
			break ;
		new_line = ft_join_free(new_line, buffer); // aqui a gente adiciona o que estiver no buffer à new_line
        if (!new_line)
            return (NULL);
        if (ft_locate_nl(buffer) >= 0) // caso uma \n tenha entrado no buffer nessa rodada, está na hora de parar de ler: temos uma linha inteira!
            break ;
		ft_memmove_nl(buffer); // para evitar lixo de memória no fim do arquivo, limpamos o buffer de novo
    }
	return (new_line);
}

char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    if (fd < 0 || BUFFER_SIZE <= 0) // saímos cedo se o usuário mandou um fd ou BUFFER_SIZE inválido
		return (NULL);

	new_line = NULL; // para evitar erros causados por lixo de memória, inicializamos new_line como NULL

    if (buffer[0] != '\0') // caso ainda haja texto salvo no buffer, adicionamos ele à new_line antes de começar a ler de novo
        new_line = ft_join_free(new_line, buffer);

    if (ft_locate_nl(buffer) >= 0)                // se já houver uma \n dentro do buffer, isso quer dizer que já há uma linha inteira
        return (ft_handle_nl(buffer, new_line));  // carregada e pronta pra ser devolvida ao usuário: podemos retornar já daqui, sem ler!

    new_line = ft_read_loop(buffer, new_line, fd); // chamamos nosso loop de leitura, que vai correr até uma de 3 coisas ocorrer:
												   // um erro de leitura, achar uma \n ou acabar o fd.
	if (!new_line)
		return (NULL);	// como read_loop faz mallocs, temos que nos proteger contra um erro de malloc

    new_line = ft_handle_nl(buffer, new_line);     // tratamos do buffer e da new_line antes de devolver
    
	return (new_line);
}
