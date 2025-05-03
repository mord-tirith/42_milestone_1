
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

Na pasta 2_Get você pode achar um programa simples que faz isso: ele abre um arquivo, salvando o endereço num fd (o arquivo em questão também está na pasta) e tenta ler um pouco de texto dele. Você pode escolher executar o programa com argumento 1, 2 ou 3 para fazer diferentes "repetições."

Estude o código e entenda como ele funciona antes de seguir adiante!

O exemplo desse programa deveria servir para mostrar algumas coisas sobre como a função _read()_ funciona:

#### Detalhes da read()

* 1º 
Você reparou algo interessante ao usar os comandos 2 ou 3 no programa? A função get_1() nunca mudou, ela sempre faz a mesma coisa: usa _read()_ para ler uma quantidade BUFFER_SIZE de caractéres para dentro da variável first_line. Porém, mesmo a função sendo sempre igual, quando você pediu para o programa rodar ela 2 ou 3 vezes seguidas, _o retorno foi diferente!_

Não só diferente, mas precisamente "a continuação de onde o read() anterior parou," em vez de ter lhe devolvido 3 vezes a mesma linha de texto.

Isso acontece porque a função read() é esperta o bastante para lembrar que já foi chamada antes nesse programa. Desde que você não tenha usado a função _close()_ para fechar o seu fd, a _read()_ vai lembrar onde parou da última vez que foi usada e, se for chamada de novo, vai continuar a ler _a partir daquele ponto._

Isso vai ser incrivelmente útil para nossa GNL, não precisamos nos preocupar em criar uma forma de rastrear o que já foi lido: a _read()_ faz isso sozinha pra gente!

* 2º
Se você rodou os testes sugeridos no get.c deve ter reparado também o útil retorno negativo, bem como retorno de 0 quando read chega no fim do arquivo inteiro.

Estes retornos nos serão indispensáveis para criar uma GNL capaz de reagir à erros de leitura e ao fim do arquivo.

Mas por hora, vamos falar do próximo passo: line.
## 3_Line

Pois é, estamos saindo da ordem! Não será "1_get", "2_next", "3_line": vamos falar de "linha" antes de nos preocuparmos com a parte mais difícil.

Da forma que nosso código está até agora, nós podemos ler, mas nunca paramos quando achamos o famigerado \n que estamos caçando.

Para isso, você tem 2 caminhos: criar uma função para caçar um \n ou, minha sugestão pessoal: adicionar seu ft_strchr da _libft_ à lista de funções no seu `get_next_line_utils.c`

A ideia agora vai ser criar uma função que, em vez de apenas ler uma quantidade de bytes do fd, vai continuar lendo até esta coleção de bytes conter o caractér \n.

Porém, surge um problema para encararmos: que tamanho vamos dar ao nosso char * que recebe o texto lido?

Antes, isso era trivial: o destino da read() precisava sempre ter tamanho "BUFFER_SIZE + 1" mas agora isso está mais complexo: e se a gente tiver que ler o buffer 3 vezes antes de achar um \n? E se tivermos que ler ele 15 mil vezes? E se lermos o fd inteiro e *nunca* acharmos um \n!?

É aqui que vamos ter um breve desvio para explorar o verdadeiro coração de uma GNL, uma função sem a qual você não vai poder seguir adiante: a ft_join_free();

### ft_join_free

Malabarismo de ponteiros é uma arte complexa e terrivelmente fácil de dar em grandes erros.

Para criar uma linha que continua aumentando à medida que você adiciona mais texto nela, você precisa criar um ponteiro só pra guardar o ponteiro original, num esquema que rapidamente sai do seu controle. Observe o exemplo adiante, onde eu quero criar uma linha de tamanho 10, depois trocar ela para tamanho 14:

```
char	*linha;

linha = malloc(10);
linha = malloc(14);

```
Quando executar o segundo malloc, você já perdeu o ponteiro original de linha. Por isso, algumas pessoas preferem fazer "malabarismo" com um ponteiro temporário:
```
char	*linha;
char	*temp;

linha = malloc(10);
temp = linha;

linha = malloc(14);
free(temp);
```
Agora, como temp guarda o endereço original de linha, você deu free no ponteiro abandonado, eba!

Mas há uma forma bem mais simples de lidar com isso: nós iremos precisar adicionar texto num char * que voltará para o usuário. Por quê não simplificar nossa vida e criar a função responsável por essa adição para _já dar free no ponteiro antigo,_ nos deixando livres para escrever o resto do código sem se preocupar com isso?

É dessa ideia que nasce a ft_join_free, uma versão "mutante" da ft_strjoin.

Um passo a passo de como escrever tal função existe na pasta 3_Line, mas eu recomendo altamente que você tente escrever sua própria versão do 0!

Bem, voltando ao assunto principal, agora que temos uma ft_join_free, podemos nos preocupar com a segunda parte implícita na definição de "line:" cortar a linha depois do \n!

### Lidando com o \n

Bem, aqui estamos: nós recebemos um fd do usuário e usamos repetidas chamadas da _read()_ e de nossa ft_join_free para criar uma super linha que contém um monte de texto... Agora, está na hora de devolver isso, mas apenas até o \n, nem um caractér a mais!

Por sorte, a estríta definição do que é uma string em C nos ajuda aqui: uma string é um array de caractéres que contém o caractér nulo (\0).

Isso significa que

`char a[] = {'A', 'm', 'o', ' ', 'G', 'N', 'L'};`

**não** é uma string, e que

`char b[] = {'A', 'm', 'o', ' ', 'G', 'N', 'L', '\0'};`

**é** uma string!

Só que fica melhor que isso:

`char c[] = {'A', 'm', 'o', ' ', 'G', 'N', 'L', '\0', 'M', 'E', 'N', 'T', 'I', 'R', 'A', '!', '!', '\0'};`

**ainda** é uma string... Só que b é igual à c! Tecnicamente, a string termina no primeiro \0 que contem. Qualquer texto sobrando depois daquele \0 faz parte do array, sim, mas _não faz parte da string!_

Se esse é um conceito novo para você, recomendo que brinque um pouco com criar arrays de char dessa forma, declarando explicitamente cara item do array em vez de usar a conveniente simplificação `char d[] = "Odeio strings!"` para confirmar que funções como strlen, strjoin, strlcat ou strcmp todas consideram que a string acabou no primeiro \0 que existia no array!

Isso significa que "se livrar do que há depois do \n" é incrivelmente fácil: precisamos apenas botar um \0 um caractér depois do \n e pronto: nós agora temos uma string que acaba 1 byte depois do \n!

### Antes do Next:

Na próxima sessão vamos começar a falar da parte realmente difícil da GNL: o "next."

Assim sendo, por favor, tome tempo para experimentar com os programas get.c e line.c nas pastas 2_get e 3_next, faça diferentes testes com eles, veja se há problemas, encontre os bugs que existem neles, tente criar sua própria versão desses programas, porquê nós vamos embarcar numa jornada de variáveis estáticas e loucura no próximo passo, então tente garantir que está com tudo em ordem nessa parte antes de seguir adiante!


