
# GNL: passo a passo
**Índice:**
- [1 - get next line-h](#1---get-next-line-h)
  * [1-a Macros](#1-a-macros)
  * [1 - b Macros alteraveis ao compilar](#1---b-macros-alteraveis-ao-compilar)
- [2 get-next-lin-c](#2-get-next-lin-c)
  * [2-a Suas variaveis](#2-a-suas-variaveis)
    + [2-a-I buffer](#2-a-i-buffer)
  * [2-b O loop central](#2-b-o-loop-central)
  * [2-c ft-handle-nl](#2-c-ft-handle-nl)
  * [2-d Next line](#2-d-next-line)
  * [2-e Todos malditos edgecases](#2-e-todos-malditos-edgecases)
    + [2-e-I Se o fd ou BUFFER-SIZE n existem](#2-e-i-se-o-fd-ou-buffer-size-n-existem)
    + [2-e-II Se o arquivo tem o tamanho exato de BUFFER-SIZE](#2-e-ii-se-o-arquivo-tem-o-tamanho-exato-de-buffer-size)
    + [2-e-III Se o aquivo tem a mesma quantidade de bytes que o BUFFER-SIZE mas tem um linebreak](#2-e-iii-se-o-aquivo-tem-a-mesma-quantidade-de-bytes-que-o-buffer-size-mas-tem-um-linebreak)
    + [2-e-IV Round Surpresa - Se eu usei ponteiro em vez de array](#2-e-iv-round-surpresa---se-eu-usei-ponteiro-em-vez-de-array)
- [3 get-next-line-utils-c](#3-get-next-line-utils-c)
  * [ft-join-free](#ft-join-free)
    + [ft-join-free-a join](#ft-join-free-a-join)
    + [ft-join-free-b free](#ft-join-free-b-free)
  * [ft-locate-nl](#ft-locate-nl)
  * [ft-memmove-nl](#ft-memmove-nl)
  * [ft-safe-free](#ft-safe-free)
- [4 get-next-line-bonus-c](#4-get-next-line-bonus-c)
- [Fin](#fin)
## 1 - get next line-h

Vamos começar do começo: o _subject_ nos avisa que vamos precisar de um macro chamado BUFFER_SIZE para rodar esse programa, e que este macro precisa ser alterável na hora de compilar o código pelo usuário para qualquer valor que deseje.

### 1-a Macros

O que nos deixa com uma pergunta: o que p**** é um "macro"?

Macros em C são "trocas de texto em tempo de compilação," uma explicação que me fez dizer "ah, claro, trocas em compilação, é óbvio agora" a primeira vez que li, mas na verdade, é isso mesmo xD

Considere o seguinte exemplo de um programa C:

```
#include <stdio.h>
#define MINHA_STRING "Hello World!"

int	main(void)
{
	printf("%s", MINHA_STRING);
	return (0);
}
```
Se executado, como você podia ter predito, isso vai resultar em "Hello World!" aparecendo no terminal, grande merda né? Mas o importante é saber COMO isso aconteceu!

Ao contrário de uma variável `char *linha = "Hello World!"` que guarda um endereço de memória onde aquela string se encontra, um macro é "substituição de texto" quando você compila o código. Isso quer dizer que, na hora que você compilou esse programa, o lugar onde "MINHA_STRING" estava escrito foi _substituído_ pelo valor do macro: "Hello World!".

Quer uma prova melhor de que isso é verdade? Considere o seguinte programa insano em C:
```
#include <stdio.h>

#define BODY printf("Hello World!");
#define MAIN int main(){
#define END }

MAIN
BODY
END
```
Pois é: essa loucura AINDA RODA sem problemas! Isso acontece porquê, como disse antes: todo lugar que um macro é usado é "substituído por aquele texto" na hora que você compila o código. Loucura, hã?

Agora, como a norminette é uma desgraçada, nós só podemos declarar macros dentro de nossos arquivos .h, nunca nos .c . Isso não é grande problema, só precisamos saber que na nossa get_next_line.h precisamos ter uma linha dizendo `# define BUFFER_SIZE 1`, com "1" sendo qualquer número que você queira. Eu pessoalmente sempre uso 42 pq, enfim né, motivos óbvios.

### 1 - b Macros alteraveis ao compilar

Mas pera, o GNL não tem um "BUFFER_SIZE que é sempre um valor": eles mandaram a função ser "ajustável pelo usuário." Como vamos fazer isso?

É aqui que entram _condicionais de headers._ A essa altura você já criou alguns headers, nem que tenha sido só o da libft.h. Lembra que você precisava criar uma condicional `#ifndef LIBFT_H` para evitar que um usuário descuidado importasse sua bibliotéca 2 vezes? Bem, vamos usar o mesmo truque aqui, e apenas dar um valor à BUFFER_SIZE se o usuário ainda não declarou um valor!

Por causa disso, sua get_next_line.h vai acabar com essa cara:
```
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

#endif
```
O primeiro ifndef declara nossa bibliotéca, o segundo checa: "alguém já decidiu o tamanho de BUFFER_SIZE? Se não, vamos dar à ele o valor 42."

Se os espaços entre as #s lhe incomoda: bem vindo ao nojo que é a norminette em arquivos .h, todos nós odiamos isso, welcome to the club.


## 2 get-next-lin-c

Agora que temos nosso BUFFER_SIZE, é hora de começarmos a verdadeira brincadeira.

Por hora, vamos _presumir_ que nossa get_next_line_utils.c terá as seguintes funções pra nos ajudar: ft_strlen, ft_join_free, ft_locate_nl e ft_memmove_nl e ft_safe_free;

Vamos criar essas helpers no próximo passo, mas para você acompanhar o tutorial, vou dizer uma idéia geral de como elas vão functionar:

* ft_strlen - por favor né, você sabe o que essa faz.
* ft_join_free - concatena 2 strings, dando free em uma delas.
* ft_locate_nl - retorna -1 se não há \n na string, ou o index do \n se existir
* ft_memmove_nl - move um bloco de memória para trás até um \n ou limpa tudo se não há \n no bloco
* ft_safe_free - essa simples função recebe o buffer e a linha que estamos construíndo, dando free na segunda e limpando a primeira com ft_memmove_nl, garantindo que nenhum lixo de memória sobra em caso de erro!

Com essas 4 helpers na nossa mente, podemos conquistar o GNL!

### 2-a Suas variaveis

Uma das maiores vantagens do método "array em vez de pointer" é quão simples sua get_next_line() vai acabar sendo. Pessoas que trabalham com ponteiro precisam ter ao menos 3, quase sempre 4, as vezes mais de 10 diferentes variáveis tipo char * para fazer o malabarismo todo.

Você vai precisar de **duas** variáveis char para criar sua get_next_line:
```
char    *get_next_line(int fd){
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;
    ssize_t     bytes_read;

    new_line = NULL;
    bytes_read = 1;
}
```
E isso é tudo!

new_line é a string que nós vamos criar aos poucos quando formos lendo do fd que o usuário mandou. Vai ser ela que vamos constantemente mandar para a nossa ft_join_free, para ser trocada por uma nova versão da new_line que contém o texto que já havia em new_line antes, bem como o que quer que esteja dentro do buffer.

Enquanto isso buffer é nossa variável estática para o projeto (nossa ÚNICA estática, pois estamos caçando 125 pontos aqui!): vai ser para dentro da buffer que vamos mandar a função _read()_ enviar o texto puxado de nosso fd e, graças ao fato de buffer ser estático e graças à nossa função ft_memmove_nl, ele será nossa chave para guardar restos de uma leitura passada.

Isso nos trás à uma importante pergunta: o que diabos é um "buffer"?

#### 2-a-I buffer

Lembra uns 10 anos atrás quando apenas os ricos tinham acesso à "internet incrivelmente rápida de 10mb/s" e você queria acessar um vídeo no YouTube ou assitir algo na Netflix, mas a cada 10 segundos de visualização a tela congelava, ficava escura e uma irritante seta começava a girar na tela, com texto dizendo "Bufferizando vídeo"?

Não? Beleza, outro exemplo: sabe quando você, _hoje em dia,_ vai no YouTube e abre um vídeo, a barra de progresso é dividida em 3 partes? Há uma parte vermelha que representa o que você já assistiu, uma parte cinzenta que vai de onde você está para a frente, e uma parte de um cinza mais claro?

_Essa parte cinza com cor mais forte_ é o buffer.

E o que é um buffer?

De forma simplista, um buffer é "o lugar onde memória fica guardada entre os passos de ser obtida e utilizada."

No exemplo do YouTube, o buffer representa a parte do vídeo que sua internet _já baixou,_ mas que você ainda não assistiu. A parte cinza mais clara, depois do buffer, representa porções do vídeo que você ainda não fez download.

Um exemplo simples de como buffers funcionam pode ser visto se você usar uma internet meio lenta (algo como o 3g de seu celular?) e abrir um vídeo no YouTube: você vai perceber que pode pular adiante no vídeo sem ter que esperar que o aplicativo carregue, _desde que seu salto tenha ocorrido dentro do buffer,_ mas se você tentar pular para fora do buffer, vai ter que esperar enquanto o YouTube carrega aquela parte do vídeo: ela não estava pronta ainda!

Eu escolho o nome "buffer" para a variável estática da get_new_line porquê é exatamente isso que ela faz: quando usamos _read()_ para ler dados do fd, estamos "bufferizando" aquela informação. Eventualmente, vamos mandar esses dados do buffer para a linha que o usuário vai receber.

Quando devolvemos a new_line para o usuário, se sobra dados no buffer, é para isso que um buffer serve: guardar memória que já foi baixada (ou "lida" em nosso caso) e que ainda não foi usada!

### 2-b O loop central

Agora que temos um buffer e uma new_line, é hora de jogar dados do fd para essa new_line!

O loop principal de uma GNL é "simples": teoricamente e estruturamente fácil, mas quando você preisa levar em conta todas possibilidades, fica meio chato de ler.

Vamos começar com o coração do loop então:
```
while (bytes_read)
{
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    new_line = ft_join_free(new_line, buffer);
}
```
Esse loop leria todo um fd para dentro da new_line: cada iteração vai ler BUFFER_SIZE bytes para dentro de buffer, depois vai usar a ft_join_free para adicionar esses bytes à new_line. 

Quando o arquivo acabar e bytes_read for igual à 0, o loop acaba.

Fácil né? Bom também não é o que precisamos. Então vamos começar a complicar nosso loop, com uma nova condição: se acharmos uma \n, isso significa que acabamos de ler uma linha inteira, e é hora de parar o loop!

```
while (bytes_read)
{
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    new_line = ft_join_free(new_line, buffer);
    if (ft_locate_nl(buffer) >= 0)
        break ;
}
```
Lembre que nossa ft_locate_nl retorna -1 se não houver nenhuma \n na string que recebe. Se você preferiu usar ft_strchr em vez de ft_locate_nl, basta usar uma condicional que diz `if (ft_strchr(buffer, '\n'))` já que ft_strchr vai retornar NULL se não encontar o char desejado na string.

Note que essa verificação é feita _depois_ de colar o que há no buffer em new_line. Ainda assim, usamos buffer para saber se apareceu um \n em vez de buffer porquê, bem, buffer sempre vai ter um tamanho igual à ou menor que new_line, então é simplesmente mais eficiente buscar nele do que na linha toda.

Agora nosso loop termina ao achar uma \n, mas ainda falta um ingrediente: parar se o fd acabou. "Mas Thiago, a condição do loop é `while (bytes_read)`, o loop já para se o fd acabar!"

Bem, lembra daquele bug estranho que acontecia no passo 3_Line do tutorial anterior quando você imprimia o arquivo long_lorem.txt com um BUFFER_SIZE grande? 

Caso não tenha experimentado com isso, volte lá e experimente agora!

Aquele erro ocorre porquê estamos copiando texto do buffer para a new_line _mesmo quando não lemos nada e o fd acabou!_ Para escapar desse erro, vamos complicar um pouco mais o nosso loop:
```
while (1)
{
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == 0)
        break ;
    new_line = ft_join_free(new_line, buffer);
    if (ft_locate_nl(buffer) >= 0)
        break ;
}
```
Agora que ambas condições onde queremos que o loop acabe estão escritas dentro do próprio loop, podemos ser mais sinceros e retirar bytes_read da condicional do loop e botar qualquer número que não seja 0: isso vai fazer o loop rodar "para sempre," o que quer dizer "rodar até bater em uma das 2 condições que criamos para fazer break.

Porém há um problema: achar um \n ou chegar ao fim do arquivo não são _todas_ condições nas quais deviamos parar de ler! Há uma terceira opção, mais cruel e misteriosa: _e se a leitura deu errado!?_

Para isso, vamos criar a condição final, e essa vai ser mais chata:
```
while (1)
{
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read < 0)
        return (ft_safe_free(new_line, buffer));
    if (bytes_read == 0)
        break ;
    new_line = ft_join_free(new_line, buffer);
    if (ft_locate_nl(buffer) >= 0)
        break ;
}
```
Agora sim! Estamos completamente protegidos contra loops infinitos: ou vamos ler até achar um \n, ou vamos ler até chegar ao fim do arquivo, ou vamos bater em um erro de leitura, dar free em new_line, limpar nosso buffer e retornar nulo!

Porém, a essa altura, nós já temos coisa demais nesse loop, e tentar fazer isso caber na norminette seria loucura.

Portanto, quase todo mundo que faz o GNL prefere jogar esse trabalho todo para uma função remota, assim:

```
static char    *ft_read_loop(char *buffer, char *new_line)
{
    ssize_t bytes_read;

    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0)
            return (ft_safe_free(new_line, buffer));
        if (bytes_read == 0)
            break ;
        new_line = ft_join_free(new_line, buffer);
        if (!new_line)
            return (NULL);
        if (ft_locate_nl(buffer) >= 0)
            break ;
    }
}

char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    new_line = NULL;
    new_line = ft_read_loop(buffer, new_line);
    retun (new_line);
}
```

Agora nossa get_next_line tem apenas 6 linhas! Esse truque será usado de novo daqui há pouco quando formos falar da ft_handle_nl. Por hora, aprecie o que temos até agora: rode este código (as 5 helper functions podem ser achadas na pasta "files" acima).

Experimente usar fds inválidos, arquivos com muito texto que nunca contém uma \n, etc!

Se divertiu? Okay, hora de adicionar uma parte vital da GNL ao nosso código: como lidamos com \n?

### 2-c ft-handle-nl

Dessa vez não vamos nem fingir que o que vamos fazer vai caber na nossa get_next_line: a norminette nos odeia demais para isso. Portanto, vamos apenas adicionar uma chamada à função que lida com \n antes de devolver a linha pro usuário:
```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    new_line = NULL;
    new_line = ft_read_loop(buffer, new_line);
    new_line = ft_handle_nl(buffer, new_line);
    retun (new_line);
}
```
E vamos pensar no que essa função precisa fazer: botar um \0 um carácter depois do primeiro \n na new_line, depois chamar nossa ft_memmove_nl para deixar o buffer pronto para a próxima chamada. Isso é fácil:

```
static char *ft_handle_nl(buffer, new_line)
{
    int nl_index;

    nl_index = ft_locate_nl(new_line);
    if (nl_index >= 0)
        new_line[nl_index + 1] = '\0';
    ft_memmove_nl(buffer);
    return (new_line);
}
```

Ótimo, simples e direto: achamos um \n em new_line? Se sim, vamos pôr um \0 um byte depois dele, e depois retornar a nova new_line!

Se rodar como está agora, vai perceber que melhoramos bastante: se tentar ler um arquivo com BUFFER_SIZE=15000, ainda assim só vai receber a primeira linha de texto: progresso!

Agora, é hora de dor de cabeça: vamos pegar a PRÓXIMA linha!

### 2-d Next line

Graças às nossas helpers e a estarmos usando um array para nosso buffer, é muito fácil descobrirmos se estamos ou não na "primeira chamada" da GNL: basta verificarmos se o que está salvo no index 0 do buffer é ou não é um caractér nulo.

Imagine esse exemplo, onde o seguinte bloco de texto entrou no buffer na chamada anterior:

`abcdefghijklmnopqrstuvwxyz\n0123456789\0`

Nossa nova ft_handle_nl já devolveu o seguinte array para nosso usuário:

`abcdefghijklmnopqrstuvwxyz\n\0123456789\0`

Que, graças ao fato de strings em C acabarem no primeiro \0 que contém, significa que o usuário recebeu a seguinte string:

`"abcdefghijklmnopqrstuvwxyz\n"`

Também, por causa de como nossa ft_memmove_nl funciona, o que ficou salvo no nosso array estático foi:

`0123456789\0`

Quando a get_next_line é chamada de novo, basta a gente verificar se o primeiro caractér no buffer é um \0 ou não e, se for, adicionar esse "resto" à new_line antes de seguirmos adiante para o loop de leitura!

```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    new_line = NULL;
    if (buffer[0] != '\0')
        new_line = ft_join_free(new_line, buffer);
    new_line = ft_read_loop(buffer, new_line);
    new_line = ft_handle_nl(buffer, new_line);
    retun (new_line);
}
```
Boom, lidamos com sobras baby!

Agora, se houver mais texto depois do fim daquela string louca, nossa get_next_line vai adicionar o que a read() encontrar _depois_ do que sobrou da última chamada!

Há um extra se você quiser:
```
if (ft_locate_nl(buffer) >= 0)
    return (ft_handle_nl(buffer, new_line));
```
Essa condicional adicionada logo depois da anterior tem um efeito lindo: ao invés de perder tempo entrando no loop, se você sabe que já há uma linha nova inteira dentro do buffer, incluíndo uma nova \n, não precisa fazer todo o trabalho da get_next_line: basta tratar esse \n com nossa ft_handle_nl e devolver o resultado pro usuário!

Isso lida com casos onde o buffer depois de uma leitura tinha algo assim salvo neles:

`abcdefghijklmnopqrstuvwxyz\n0123456789\nABCDEF`

Quando devolvemos a primeira string pro usuário, isso sobrou no buffer:

`0123456789\nABCDEF`

Com essa nova verificação adicionada, nossa get_next_line vai perceber: "epa, já há uma \n nova no buffer atual, só preciso devolver '0123456789\n' pro usuário e meu trabalho acabou" sem perder tempo!

Portanto, nossa get_next_line acaba sendo:

```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    new_line = NULL;
    if (buffer[0] != '\0')
        new_line = ft_join_free(new_line, buffer);
    if (ft_locate_nl(buffer) >= 0)
        return (ft_handle_nl(buffer, new_line));
    new_line = ft_read_loop(buffer, new_line);
    new_line = ft_handle_nl(buffer, new_line);
    retun (new_line);
}
```

E nosso trabalho está 99.9% completo!

### 2-e Todos malditos edgecases

Há um milhão de edgecases quando se trabalha com GNL: exemplos de uso estranhos e/ou extremos nos quais sua GNL ainda deve retornar a linha sem problemas. Vamos começar a caçar esses um por um!

#### 2-e-I Se o fd ou BUFFER-SIZE n existem

Uma pessoa *normal* usaria um comando do tipo `int fd = open("arquivo.txt", O_RDONLY)` para gerar um fd que vai usar na sua GNL. Mas e se a pessoa decidir pedir para sua função fazer algo como `char *line = get_new_line(-42)`? Pior, e se um usuário que está usando sua GNL em total boa fé tenta executar o primeiro comando, mas esqueceu que o nome do arquivo era "Arquivo.txt" em vez de "arquivo.txt"?

Nós _podemos_ criar várias precauções dentro do nosso loop de leitura na ft_read_loop, mas é mais fácil fazer isso no começo: vamos trocar o começo de nossa get_next_line() de 

```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    new_line = NULL;
```

para:

```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    if (fd < 0)
        return (NULL);
    new_line = NULL;
```

Mas... E se o usuário por acidente meteu um "-1" ou "-42"? Bom, vamos nos proteger contra isso também: `if (fd < 0 || BUFFER_SIZE <= 0)` agora que nossa função retorna _NULL_ se o fd ou BUFFER_SIZE forem inválidos, estamos protegidos contra maus usos!

#### 2-e-II Se o arquivo tem o tamanho exato de BUFFER-SIZE

Essa é uma pergunta cruel, criada especificamente para nos atormentar.

Até agora, nós temos presumido que a função _read()_ vai "escrever por cima" do nosso buffer, e em 99% dos casos, é exatamente isso que ela faz!

Porém, quando _read()_ chega ao fim do arquivo, **ela para de ler sem tocar no buffer!**

Isso faz com que um arquivo com o seguinte conteúdo: `0123456789` se comporte assim se acessado com BUFFER_SIZE = 10:

* 1 - lemos 10 bytes do arquivo para o buffer, deixando isso no buffer:
`0 1 2 3 4 5 6 7 8 9 \0`
* 2 - devolvemos "0123456789" para o usuário como a primeira linha. Até aqui, tudo em ordem!
* 3 - O usuário chama GNL de novo... Agora o problema começa! O caractér '0' está no começo de nosso buffer, então nossa função vai adicionar "0123456789" à nova linha de novo, oh não!

Para se proteger contra isso, precisamos adicionar uma limpeza ao nosso buffer quando mandamos dados do buffer para a new_line, mas só se o buffer inteiro foi para ela!

Isso parece super complexo, mas na verdade é super simples, basta adicionar uma chamada à nossa ft_memmove_nl no nosso loop de leitura quando sabemos que vamos ler de novo:

```
    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0)
            return (ft_safe_free(new_line, buffer));
        if (bytes_read == 0)
            break ;
        new_line = ft_join_free(new_line, buffer);
        if (!new_line)
            return (NULL);
        if (ft_locate_nl(buffer) >= 0)
            break ;
        ft_memmove_nl(buffer);
    }
```

E isso... _quase_ acaba os edgecases!

#### 2-e-III Se o aquivo tem a mesma quantidade de bytes que o BUFFER-SIZE mas tem um linebreak

Deus do céu de onde esses sadistas inventam esses testes!?

Consegue entender o problema? Nós agora limpamos nosso buffer antes de ler novamente, uma ótima idéia, mas essa limpeza tem que acontecer _depois_ de sabermos que vamos rodar o loop de novo... Mas as condições pra saber se vamos rodar o loop de novo incluem um \n, e agora!?

A primeira vez que entendi que era isso que os testes "giant_line_nl" e "alternate_line_nl_with_nl" da francinette estavam testando, eu quis desistir de tudo. Por sorte, a solução era mais fácil do que meu drama na hora me fez imaginar: nós só entramos na função ft_read_loop se já tratamos de restos no buffer. Assim sendo, basta limparmos o buffer antes de entrar no loop de leitura!

```
    ft_memmove_nl(buffer);
    while (1)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0)
            return (ft_safe_free(new_line, buffer));
        if (bytes_read == 0)
            break ;
        new_line = ft_join_free(new_line, buffer);
        if (!new_line)
            return (NULL);
        if (ft_locate_nl(buffer) >= 0)
            break ;
        ft_memmove_nl(buffer);
    }
```

Isso é seguro, porquê nessa altura, uma das duas condições são verdadeiras: ou havia coisas salvas no buffer e nossa condicional `if (buffer[0] != '\0'` já tratou de salvar o que estava lá dentro de nossa new_line, ou o buffer está vazio, caso no qual não importa muito se nós limpamos ele ou não!

Com essas últimas alterações, nós terminamos nosso get_next_line.c. Caso queira dar uma olhada, a pasta files acima contém um arquivo chamado complete_gnl.c que contém esse código.

#### 2-e-IV Round Surpresa - Se eu usei ponteiro em vez de array

Nessa altura, tenho que lhe abandonar. Ao meu saber, existem algumas dezenas de edgecases que só podem ocorrer se você escolheu usar vários ponteiros em vez de um array para seu buffer. A melhor forma de lidar com a lista de problemas causados por essa escolha me foi sugerida pelo aluno lgetrud: verifique todos lugares em que você faz uma cechagem se um malloc deu errado, o clássico
```
char    *ptr;

ptr = malloc(1);
if (!ptr)
    return (NULL);
```
E, uma de cada vez, troque as condicionais de `if (!ptr)` para `if (ptr)`: isso vai simular um erro de malloc em cada um dos seus mallocs. Sua get_next_line deveria retornar _NULL_ em todos esses casos, mas é provável que algum deles cause um erro de segmentação em vez de um retorno de nulo: ache os que estão causando isso, e você achou um dos edgecases criados por ter usado ponteiros.

Daí em diante, boa sorte para reescrever o código para evitar esse erro.

Se você está recebendo vários erros de "NULL_CHECK" na francinette (um erro que só pode ocorrer com os que usam ponteiros em vez de array...), esse é quase certamente o motivo: cace esses edgecases e passe logo desse projeto!

## 3 get-next-line-utils-c

Vamos lá, reta final!

Nosso códio na get_new_line.c depende de 5 helper functions para correr sem problemas. Uma delas você pode copiar direto da libft: ft_strlen, então não vamos nem falar dela.

Quanto às outras, nós temos que criar:

* ft_join_free - uma versão especial da ft_strjoin.
* ft_locate_nl - uma versão estranha da ft_strchr.
* ft_memmove_nl - talvez a mais estranha de todas, nossa "limpadora de buffer"
* ft_safe_free - uma economizadora de linhas xD

Sinceramente, se você conhece alguns truques para reduzir linhas de código, eu diria que nem precisa da ft_safe_free, e se souber como fazer matemática com ponteiros nem precisa da ft_locate_nl e ft_strlen. Eu decidi criar a GNL com essas 5 para maximizar legibilidade do código, mas se quer ver uma coisa um pouco absurda, olhe o monstro de Frankenstein possuído pelo demônio que eu enviei na pasta GNL_0.

Sem mais delongas, vamos começar!

### ft-join-free

O coração de uma boa GNL, ft_join_free tem um propósito bem direto: vamos receber nosso buffer e a linha que estamos aos poucos construíndo para devolver para o usuário. O trabalho da ft_join_free é concatenar o que estiver em buffer para o fim da new_line, criando uma nova string, antes de dar free na new_line antiga e devolver o resultado.

Vamos começar por partes então:

#### ft-join-free-a join

Ao contrário do inferno que sofremos na ft_strlcat durante a libft, concatenar uma string 
à outra é bastante simples, só precisamos de dois iteradores para copiar, char por char, das strings para nossa nova linha, em ordem!

```
char    *bigger_line;
ssize_t i;
ssize_t j;

big_line = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buffer) + 1));
i = -1;
j = -1;
```

O -1 nos dois iteradores é um truque para nos economizar 3 linhas, pois se começamos eles como -1 nós podemos aumentar o iterador na própria condição do loop, tornando algo como
```
while (new_line[i])
{
    bigger_line[i] = new_line[i];
    i++;
}
```
No loop super simplificado:
```
while (new_line[++i])
    bigger_line[i] = new_line[i];
```

ft_join_free é de longe a maior função que vamos construír nesse projeto, então toda e qualquer linha que podemos salvar nos ajuda a agradar a norminette.

Agora que temos nossas 3 variáveis necessárias criadas e inicializadas, podemos fazer o trabalho de concatenar:
```
while (new_line[++i])
    bigger_line[i] = new_line[i];
while (buffer[++j])
    bigger_line[i + j] = buffer[j];
bigger_line[i + j] = '\0';
```
ESPERA! 
"bigger_line" foi criada com um malloc, e aqui estamos nós, mandando o programa escrever chars dentro dela, presumindo que nosso malloc deu certo, isso não pode! Então, vamos encerrar o trabalho de concatenação, prendendo o processo dentro de uma condicional que só permite ele acontecer se o malloc foi bem sucedido:

```
char	*ft_join_free(char *line, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*big_line;

	big_line = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buffer) + 1));
	if (big_line)
	{
		i = -1;
		j = -1;
		while (line[++i])
			big_line[i] = line[i];
		while (buffer[++j])
			big_line[i + j] = buffer[j];
		big_line[i + j] = '\0';
	}
    return (big_line);
}
```

Agora sim, estamos seguros, e já fizemos boa parte do trabalho: criamos uma nova linha, uma que contém tudo que estava dentro da new_line antes _e_ o que quer que estivesse dentro do buffer!

#### ft-join-free-b free

Eu _queria_ dizer que essa parte do trabalho é simples, basta jogar um `free(new_line)` logo antes do retorno, mas é um pouco mais chato que isso: nós, como coders responsáveis, criamos new_line e inicializamos ela como `new_line = NULL` na nossa get_new_line, para evitar acidentes com lixo de memória.

Porém, isso quer dizer que, algumas vezes, nossa ft_join_free vai estar lidando com uma linha que não existe, e aqui estamos nós, acessando endereços de memória, como um bando de idiotas!

Por sorte, corrigir esse problema não é tão complexo, basta a gente garantir que a variável new_line _sempre_ existe, verificando se ela é nula no começo da função:
```
if (!new_line)
    new_line = malloc(1);
```
E... Opa, pera, ah não: fizemos um malloc, precisamos ter certeza que ele funcionou x.x

E é assim que essa estranha e aparentemente repetitiva condicional é criada:

```
if (!new_line)
{
    new_line = malloc(1);
    if (!new_line)
        return (NULL);
    new_line[0] = '\0';
}
```
Ufa... Agora sim, podemos considerar nossa ft_join_free completa:

```
char	*ft_join_free(char *line, char *buffer)
{
	size_t	i;
	size_t	j;
	char	*big_line;

	if (!line)
	{
		line = malloc(1);
		if (!line)
			return (NULL);
		line[0] = '\0';
	}
	big_line = malloc(sizeof(char) * (ft_strlen(line) + ft_strlen(buffer) + 1));
	if (big_line)
	{
		i = -1;
		j = -1;
		while (line[++i])
			big_line[i] = line[i];
		while (buffer[++j])
			big_line[i + j] = buffer[j];
		big_line[i + j] = '\0';
	}
	free(line);
	return (big_line);
}
```

### ft-locate-nl

Depois da aventura que foi a ft_join_free, ft_locate_nl é meio sem graça... Tudo que essa função precisa fazer é dizer para quem a chamou em que index da string recebida se encontra o primeiro char \n. Caso a ft_locate_nl percorra a string inteira e não encontre um \n, ela vai retornar -1 (ela podia retornar qualquer valor negativo, mas esse específico -1 é uma ferramenta surpresa que nos ajudará mais tarde!)

Essencialmente, ela é quase _igual_ à ft_strchr, só que em vez de devolver um ponteiro para o char buscado ou NULL se o char não se encontra na string, ela devolve um index ou um -1. Além disso, ao contrário da ft_strchr, nossa ft_locate_nl não aceita outros chars como alvo da busca: é \n ou nada!

```
ssize_t	ft_locate_nl(char *str)
{
	ssize_t	i;

	if (!str)
		return (-1);
	i = 0;
}
```
Como vamos lidar com strings, é ideal usarmos size_t em vez de int para nosso iterador 

Caso nunca tenha entendido o motivo disso, é simples: strings possuem "SIZE_MAX" como seu limite de caractéres. Isso quer dizer que uma string pode muito bem ter 2147483648 caractéres nela, se SIZE_MAX for grande o bastante pra isso em seu sistema. Se você usar int em vez de size_t, essa hipotética super string causaria problemas!

Enfim, como nós precisamos poder retornar valores negativos também, vamos usar ssize_t!

Além disso, só por segurança, vamos checar se str não é nula antes de começar a fuçar dentro dela.

Isso feito, o resto é banal:
```
while (str[i])
{
    if (str[i] == '\n')
        return (i);
    i++;
}
return (-1);
```

### ft-memmove-nl

Okay, hora da minha função favorita nessa loucura toda. Nossa ft_memmove_nl tem 2 propósitos, dependendo de 2 situações: 

* 1: mover tudo que está depois de um \n no buffer para o começo dele, ou
* 2: limpar o buffer inteiro, se não há nenhum \n nele.

O motivo disso é simples: se nós já escrevemos o que estava no buffer para a new_line e um \n estava no meio dessas coisas, nós precisamos garantir que, se o usuário chamar get_new_line de novo, apenas o que estava depois daquele \n no buffer vai ser adicionado à nova linha gerada.

Num exemplo simples, imagine que nosso buffer tinha o seguinte conteúdo após uma leitura:

`Strange, but it's true:\nI can't get ove\0`

Nós vamos devolver "Strange, but it's true\n" para o usuário, sem mistérios aqui, mas precisamos guardar "I can't get ove\0", se não a próxima vez que o usuário chamar get_next_line ele vai receber um cortado "r the way you love me like you do\n" em vez da letra correta da música do Queen, "I can't get over the way you love me like you do\n".

Então nós precisamos de duas coisas: um iterador que aponte para o começo do buffer, e um iterador que aponte para onde achamos o \n nele!

Vamos inicializar eles assim:
```
void	ft_memmove_nl(char *buffer)
{
	ssize_t	i;
	ssize_t	nli;

	i = 0;
	nli = ft_locate_nl(buffer);
```
Usei "nli" como nome do segundo iterador, podemos chamar ele de "new_line_index." Após essa inicialização, esses são os lugares onde nossos iteradores apontam:

```
Strange, but it's true:\nI can't get ove\0
i                      nli
```
Daqui em diante, só precisamos copiar o que está escrito no índice nli para o índice i, correto? ERRADO!

Lembre, que nós temos que apagar até _um char depois da \n!_ Bom, sem drama, só precisamos inicializar nli assim: `nli = ft_locate_nl + 1`.

Melhor parte: lembra como eu disse que "retornar exatamente -1 na ft_locate_nl ia nos ajudar mais tarde"? Bem, essa é a hora! Se não há nenhum \n em nosso buffer, nli vai ser inicializada com valor _0_ em vez de _-1_ graças à essa mudança. Isso quer dizer que nós podemos usar nli para verificar se estamos usando ft_memmove_nl para "puxar o buffer para trás" ou para "limpar tudo que está escrito nele"!

Armados com nossa nova nli, "puxar o buffer para o começo" é simples:
```
while (buffer[nli] && nli)
{
    buffer[i] = buffer[nli];
    nli++;
    i++;
}
```
Simples, eu disse =)

Quando nli chegar no fim do buffer, isso quer dizer que "o texto que estava no buffer depois da \n" acabou de ser copiado para o começo. No nosso exemplo, esse seria nosso buffer depois desse loop:

`I can't get ove's true:\nI can't get ove\0`

Agora que acabamos de copiar de depois da \n para o começo, só precisamos limpar o resto do buffer:

```
while (buffer[i])
{
    buffer[i] = '\0';
    i++;
}
```
Repare que, quando nli é inicializada com valor 0 (produto de ft_locate_nl ter retornado -1) nós pulamos direto para essa parte do loop, com i em valor 0: limpeza completa!

Isso _deveria_ ser o bastante, mas por pura paranóia, eu sempre me pergunto "e se houver um \0 solto no meio do meu buffer?" Eu não sei explicar **como** um \0 teria parado lá, mas nunca se sabe! Então, só para ter 100% de certeza que o buffer foi propriamente limpo, eu prefiro usar BUFFER_SIZE nessa condicional: `while (i < BUFFER_SIZE)` isso garante, sem sombra de dúvida alguma, que mesmo que nosso buffer tivesse `abc\0 Muahaha coisas escondidas!\0` escrito dentro dele, nós _ainda_ limparíamos ele todo. 

No fim das contas, nossa ft_memmove_nl acaba com esse conteúdo:
```
void	ft_memmove_nl(char *buffer)
{
	ssize_t	i;
	ssize_t	nli;

	i = 0;
	nli = ft_locate_nl(buffer) + 1;
	while (buffer[nli] && nli)
	{
		buffer[i] = buffer[nli];
		i++;
        nli++;
	}
	while (i < BUFFER_SIZE)
	{
		buffer[i] = '\0';
		i++;
	}
}
```

### ft-safe-free

Última parada: ft_safe_free. Sinceramente, você nem precisa dessa, é outro produto de paranóia minha.

ft_safe_free tem um trabalho simples: garantir, sem sombra de dúvida alguma, que nós retornamos NULL e deixamos um buffer 100% limpo para trás quando ocorre um erro de malloc ou de leitura.

Primeiro, fazemos isso dando um "free seguro" em new_line:

```
if (new_line)
    free(new_line);
```

Depois disso, temos que garantir que o buffer está completamente limpo:

```
size_t  i;

i = 0;
while (i < BUFFER_SIZE)
{
    buffer[i] = '\0';
    i++;
}
```

E isso é tudo, completando a função com:

```
char	*ft_safe_free(char *line, char *buffer)
{
	size_t	i;

	if (line)
		free(line);
	i = 0;
	while (i < BUFFER_SIZE)
	{
		buffer[i] = '\0';
		i++;
	}
	return (NULL);
}
```

Novamente, você provavelmente pode pular essa, um simples "if (new_line) free(new_line)" deveria ser o bastante em erros de leitura mas bem, eu sou paranóico xD

## 4 get-next-line-bonus-c

Bem, agora chegamos na parte mais hilária dessa jornada: se você criou seu GNL com um array em vez de ponteiros, fazer os bônus é a coisa mais simples do universo.

get_next_line convenientemente recebe uma int como argumento, "int fd." Para fazer sua GNL funcionar com vários fds diferentes em vez de apenas um, basta declarar seu buffer como um array de arrays. Isso é tudo!

```
char    *get_next_line(int fd)
{
    static char buffer[5][BUFFER_SIZE + 1];
```
Este é o exemplo de uma get_next_line que seria capaz de lidar com 2 arquivos ao mesmo tempo: se o usuário chamar a GNL com os fds de 0 à 2 ou com 2 fds extras para 2 arquivos, basta usar a variável int fd como index no nosso buffer para acessar "o buffer correto para este fd."

Claro, 5 é um limite um pouco sem graça. Pesquisando online, você vai descobrir que "em quase todos sistemas, programas têm um limite de 1024 fds abertos." Portanto, se quiser que sua GNL funcione em quase qualquer sistema, basta criar `static char buffer[1024][BUFFER_SIZE + 1]`. Melhor ainda, que tal criar um novo macro em sua get_next_line_bonus.h:
```
# ifndef FD_LIMIT
#  define FD_LIMIT 1024
# endif
```
E agora você pode criar seu buffer bonus como `static char buffer[FD_LIMIT][BUFFER_SIZE + 1]` e boom! Não só sua função vai pegar em quase todos sistemas, no raro caso de um usuário estar num sistema que permite mais (ou menos!) que 1024 fds, basta ele compilar o código com `-D FD_LIMIT=2048` ou o que quer que seja o novo limite, e sua GNL vai funcionar perfeitamente!

Claro, antes de encerrar tudo, lembre que você tem que usar o fd para indexar o buffer correto:
```
char    *get_next_line(int fd)
{
    static char buffer[FD_LIMIT][BUFFER_SIZE + 1];
    char        *new_line;

    if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line = NULL;
    if (buffer[fd][0] != '\0')
        new_line = ft_join_free(new_line, buffer[fd]);
    if (ft_locate_nl(buffer[fd]) >= 0)
        return (ft_handle_nl(buffer[fd], new_line));
    new_line = ft_read_loop(buffer[fd], new_line, fd);
    new_line = ft_handle_nl(buffer[fd], new_line);
    return (new_line);
}
```
E isso é tudo, aproveite os 25 pontos extras mais fáceis de sua vida!

Como todas funções fora da get_next_line dependem de receber uma string "buffer," você não precisa fazer mais nenhuma alteração: o resto do código já funciona. Funciona tão bem que eu honestamente _esqueci_ de trocar o #include da minha get_next_line_utils_bonus.c para incluír a get_next_line_bonus.h em vez da get_next_line.h, e ainda assim o código pegou e passou pela Moulinette sem problemas xD

## Fin

E aqui estamos... 

Foi uma jornada cruel e longa, mas chegamos ao fim!

Aprendemos sobre variáveis estáticas, caçamos edgecases, perdemos a sanidade um pouco (ou um muito em meu caso)...

No geral, GNL é meu projeto favorito da Milestone 1, o que pode ou pode não ser mero resultado de síndrome de Estocolmo.

Caso você tenha alguma dúvida, reclamação, queira me xingar por usar array em vez de ponteiros ou encontre algum problema no código, não hesite em me contactar no slack: thenriqu !
