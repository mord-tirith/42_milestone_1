
## 1 Entendendo seu projeto:

_Get_Next_Line_ é um programa bastante auto explicativo. Ao contrário de outros projetos que veremos mais adiante na _Milestone_ 2, este é bem "faz o que diz que faz": temos que criar uma função que "pega a próxima linha" de um arquivo.

O protótipo da função tem que ser:

**`char	*get_next_line(int fd);`**

Quebrando em partes para entender tudo, podemos ver que estamos lidando com:

#### Retorno:

`char*` - A função devolve um ponteiro de char, ou seja, "uma _string_" que consiste da próxima linha de texto.

#### Parâmetros:
`int fd` - A função recebe um _File Descriptor_ para saber onde vai buscar esta "próxima linha."

Além do protótipo, o _Subject_ da GNL foi legal o bastante para nos avisar que vamos ter que usar a função _read()_ para ler este fd. Então também é bom darmos uma olhadinha no que a tal da _read()_ faz antes de seguir adiante:

**`ssize_t	read(int fd, void *buffer, size_t nbytes):`**

Vamos quebrar ela em partes também, já que é um pouco mais complexa:

#### Retorno:
`ssize_t` - Para quem não usou ele na _libft_, bem vindo ao _ssize_t_! Este tipo de variável é parecido com a _size_t_ que usamos para controlar _strings_ com segurança na _libft_, mas um pouco diferente: _ssize_t_ é capaz de guardar números negativos além de positivos, ao contrário da _size_t_.

_read()_ precisa desse tipo de retorno porquê ela normalmente lhe devolve "a quantidade de bytes que foram lidos do arquivo indicado," mas se acontecer algum erro de leitura a _read()_ lhe devolve um valor negativo para especificar qual erro aconteceu.

Quando tudo corre bem, o retorno é apenas a quantidade de bytes bem lidos. Isso quase sempre vai ser igual ao valor que você colocou em _nbytes_ mas se você mandar _read()_ ler 1.000.000 de bytes de um arquivo que tem apenas 2 caracteres escritos nele, _read()_ vai lhe retornar "2" em vez de um milhão: não importa quanto você tentou ler, o que realmente havia para ser lido no arquivo eram apenas 2 bytes.

Caso _read()_ perceba que chegou no fim do arquivo, ela vai retornar 0.

#### Parâmetros:
`int fd` - Esse é fácil: qual arquivo estamos lendo? É o terminal (1), o que o usuário está escrevendo (0), uma mensagem de erro (2) ou um novo arquivo que o programa abriu (3, 4,..)?

`void *buffer` - Se prepare para ouvir a palavra "buffer" um quadrilhão de vezes durante este projeto. Por hora basta saber que este `void *buffer` é "onde _read()_ vai guardar o resultado da sua leitura:" o que quer que exista no arquivo será copiado para dentro desse buffer.

Você pode ter reparado que "hey, espera aí, o segundo argumento da _write()_ TAMBÉM é `void *buffer`," e se você pensou isso, parabéns, ganha um biscoito! _read()_ e _write()_ foram criadas juntas e para serem usadas juntas. Originalmente, você usaria _read()_ para jogar dados de um arquivo para dentro de um buffer, depois usaria _write()_ para jogar os dados daquele buffer para outro arquivo!

`size_t nbytes` - Por fim, o último parâmetro da _read()_: quantos bytes você quer que _read()_ tente ler daquele fd.

Para nossa sorte (?) este é bem simples no GNL: você sempre irá usar o macro **BUFFER_SIZE** como sua nbytes nesse projeto, então não precisa se preocupar com esse parâmetro.

Bem, agora que temos uma noção geral das partes de nossa função GNL e da função _read()_ que iremos utilizar, vamos começar a falar de escrever nosso código!


### 1.a A guerra dos ponteiros

Uma breve mensagem do autor para você: este tutorial leva em consideração, sem nenhuma pretenção, que uma GNL que utiliza um ponteiro para manter sua variável estática é inquestionavelmente 100% _errada,_ e merece uma nota 0 em avaliações por motivo de _Memory Leak._

Assim sendo, este tutorial será completamente voltado à criação de uma GNL que trabalha com uma única variável estática, um array, **não** um ponteiro.

Caso você prefira fazer sua GNL com ponteiros e esteja disposto a jogar os dados que ninguém que lhe avaliará vai pensar em rodar valgrind num teste que não percorra o arquivo inteiro, talvez você queira sair deste tutorial agora.

Ou talvez não, mesmo que você esteja do outro lado dessa guerra, acho que há coisas boas para se aprender aqui, então você que sabe!
## 2 Get

Vamos começar simples: o que é "get"?

Bem, dicionários vão lhe dizer que o verbo significa "pegar" mas isso é meio inútil, então vamos pensar no escopo de nosso projeto: temos que "pegar" uma linha de texto de um arquivo.
