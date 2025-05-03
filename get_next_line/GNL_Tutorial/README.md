

1 »»» Entendendo seu projeto:

Get_Next_Line é um programa bastante auto explicativo. Ao contrário de outros projetos que veremos mais adiante na Milestone 2, este é bem "faz o que diz que faz": temos que criar uma função que "pega a próxima linha" de um arquivo.

O protótipo da função tem que ser:

char	*get_next_line(int fd);

Quebrando em partes para entender tudo, podemos ver que estamos lidando com:

# Retorno:
			char	* - A função devolve um ponteiro de char, ou seja, "uma string" que consiste da próxima linha de texto.

# Parâmetros:
			int 	fd - A função recebe um File Descriptor para saber onde vai buscar esta "próxima linha."

Além do protótipo, o Subject da GNL foi legal o bastante para nos avisar que vamos ter que usar a função read() para ler este fd. Então também é bom darmos uma olhadinha no que a tal da read() faz antes de seguir adiante:

ssize_t	read(int fd, void *buffer, size_t nbytes):

Vamos quebrar ela em partes porquê é um pouco mais complexa:

# Retorno:
			ssize_t - Para quem não usou ele na libft, bem vindo ao ssize_t! Este tipo de variável é parecido com size_t que usamos para controlar strings com segurança na libft, mas um pouco diferente: ssize_t é capaz de guardar números negativos além de positivos, ao contrário da size_t.

read() precisa desse tipo de retorno porquê ela normalmente lhe devolve "a quantidade de bytes que foram lidos do arquivo indicado," mas se acontecer algum erro de leitura a read() lhe devolve um valor negativo para especificar qual erro aconteceu.

Quando tudo corre bem, o retorno é apenas a quantidade de bytes bem lidos. Isso quase sempre vai ser igual ao valor que você colocou em "nbytes," mas se você mandar read() ler 1.000.000 de bytes de um arquivo que tem apenas 2 caracteres escritos nele, read vai lhe retornar "2" em vez de um milhão: não importa quanto você tentou ler, o que realmente havia para ser lido no arquivo era apenas 2 bytes.

# Parâmetros:
			int		fd - Esse é fácil: qual arquivo estamos lendo? É o terminal (1), o que o usuário está escrevendo (0), uma mensagem de erro (2) ou um novo arquivo que o programa abriu (3, 4,..)?

			void	*buffer - Se prepare para ouvir a palavra "buffer" um quadrilhão de vezes durante este projeto. Há até um arquivo extra, "1_Extras_Buffer" onde eu apenas discuto "o que é um buffer." Por hora basta saber que este void *buffer é "onde read vai guardar o resultado da sua leitura:" o que quer que exista no arquivo será copiado para dentro desse buffer.

Você pode ter reparado que "hey, espera aí, o segundo argumento da write() TAMBÉM é void *buffer," e se você pensou isso, parabéns, ganha um biscoito! read() e write() foram criadas juntas e para serem usadas juntas. Originalmente, você usaria read() para jogar dados de um arquivo para dentro de um buffer, depois usaria write() para jogar os dados daquele buffer para outro arquivo!

			size_t	nbytes - Por fim, o último parâmetro da read(): quantos bytes você quer que read() tente ler daquele fd.

Para nossa sorte (?) este é bem simples no GNL: você sempre irá usar o macro BUFFER_SIZE como sua nbytes nesse projeto, então não precisa se preocupar com esse parâmetro.



Bem, agora que temos uma noção geral das partes de nossa função GNL e da função read() que iremos utilizar, vamos começar a falar de escrever nosso código!
