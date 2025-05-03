
# GNL: passo à passo!


## 1 - get_next_line.h - Parte 1!

Vamos começar do começo: o _subject_ nos avisa que vamos precisar de um macro chamado BUFFER_SIZE para rodar esse programa, e que este macro precisa ser alterável na hora de compilar o código pelo usuário para qualquer valor que deseje.

### 1.a - Macros

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

### 1.b - Macros alteráveis ao compilar

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


## 2 - get_next_line.c

Agora que temos nosso BUFFER_SIZE, é hora de começarmos a verdadeira brincadeira.

Por hora, vamos _presumir_ que nossa get_next_line_utils.c terá as seguintes funções pra nos ajudar: ft_strlen, ft_join_free, ft_locate_nl e ft_memmove_nl e ft_safe_free;

Vamos criar essas helpers no próximo passo, mas para você acompanhar o tutorial, vou dizer uma idéia geral de como elas vão functionar:

* ft_strlen - por favor né, você sabe o que essa faz.
* ft_join_free - concatena 2 strings, dando free em uma delas.
* ft_locate_nl - retorna -1 se não há \n na string, ou o index do \n se existir
* ft_memmove_nl - move um bloco de memória para trás até um \n ou limpa tudo se não há \n no bloco
* ft_safe_free - essa simples função recebe o buffer e a linha que estamos construíndo, dando free na segunda e limpando a primeira com ft_memmove_nl, garantindo que nenhum lixo de memória sobra em caso de erro!

Com essas 4 helpers na nossa mente, podemos conquistar o GNL!

### 2.a - Suas variáveis

Uma das maiores vantagens do método "array em vez de pointer" é quão simples sua get_next_line() vai acabar sendo. Pessoas que trabalham com ponteiro precisam ter ao menos 3, quase sempre 4, as vezes mais de 10 diferentes variáveis tipo char * para fazer o malabarismo todo.

Você vai precisar de **duas** variáveis char para criar sua get_next_line:
```
char    *get_next_line(int fd){
    static char buffer[BUFFER_SIZE + 1];
    char        *next_line;
    ssize_t     bytes_read;

    next_line = NULL;
    bytes_read = 1;
}
```
E isso é tudo!

next_line é a string que nós vamos criar aos poucos quando formos lendo do fd que o usuário mandou. Vai ser ela que vamos constantemente mandar para a nossa ft_join_free, para ser trocada por uma nova versão da next_line que contém o texto que já havia em next_line antes, bem como o que quer que esteja dentro do buffer.

Enquanto isso buffer é nossa variável estática para o projeto (nossa ÚNICA estática, pois estamos caçando 125 pontos aqui!): vai ser para dentro da buffer que vamos mandar a função _read()_ enviar o texto puxado de nosso fd e, graças ao fato de buffer ser estático e graças à nossa função ft_memmove_nl, ele será nossa chave para guardar restos de uma leitura passada.

Isso nos trás à uma importante pergunta: o que diabos é um "buffer"?

#### 2.a.I "buffer"

Lembra uns 10 anos atrás quando apenas os ricos tinham acesso à "internet incrivelmente rápida de 10mb/s" e você queria acessar um vídeo no YouTube ou assitir algo na Netflix, mas a cada 10 segundos de visualização a tela congelava, ficava escura e uma irritante seta começava a girar na tela, com texto dizendo "Bufferizando vídeo"?

Não? Beleza, outro exemplo: sabe quando você, _hoje em dia,_ vai no YouTube e abre um vídeo, a barra de progresso é dividida em 3 partes? Há uma parte vermelha que representa o que você já assistiu, uma parte cinzenta que vai de onde você está para a frente, e uma parte de um cinza mais claro?

_Essa parte cinza com cor mais forte_ é o buffer.

E o que é um buffer?

De forma simplista, um buffer é "o lugar onde memória fica guardada entre os passos de ser obtida e utilizada."

No exemplo do YouTube, o buffer representa a parte do vídeo que sua internet _já baixou,_ mas que você ainda não assistiu. A parte cinza mais clara, depois do buffer, representa porções do vídeo que você ainda não fez download.

Um exemplo simples de como buffers funcionam pode ser visto se você usar uma internet meio lenta (algo como o 3g de seu celular?) e abrir um vídeo no YouTube: você vai perceber que pode pular adiante no vídeo sem ter que esperar que o aplicativo carregue, _desde que seu salto tenha ocorrido dentro do buffer,_ mas se você tentar pular para fora do buffer, vai ter que esperar enquanto o YouTube carrega aquela parte do vídeo: ela não estava pronta ainda!

Eu escolho o nome "buffer" para a variável estática da get_next_line porquê é exatamente isso que ela faz: quando usamos _read()_ para ler dados do fd, estamos "bufferizando" aquela informação. Eventualmente, vamos mandar esses dados do buffer para a linha que o usuário vai receber.

Quando devolvemos a next_line para o usuário, se sobra dados no buffer, é para isso que um buffer serve: guardar memória que já foi baixada (ou "lida" em nosso caso) e que ainda não foi usada!

#### 2.b - O loop central

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
        return (ft_safe_free(buffer, new_line));
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
            return (ft_safe_free(buffer, new_line)));
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

    next_line = NULL;
    next_line = ft_read_loop(buffer, new_line);
    retun (next_line);
}
```

Agora nossa get_next_line tem apenas 6 linhas! Esse truque será usado de novo daqui há pouco quando formos falar da ft_handle_nl. Por hora, aprecie o que temos até agora: rode este código (as 5 helper functions podem ser achadas na pasta "files" acima).

Experimente usar fds inválidos, arquivos com muito texto que nunca contém uma \n, etc!

Se divertiu? Okay, hora de adicionar uma parte vital da GNL ao nosso código: como lidamos com \n?

#### 2.c - ft_handle_nl

Dessa vez não vamos nem fingir que o que vamos fazer vai caber na nossa get_next_line: a norminette nos odeia demais para isso. Portanto, vamos apenas adicionar uma chamada à função que lida com \n antes de devolver a linha pro usuário:
```
char    *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    char        *new_line;

    next_line = NULL;
    next_line = ft_read_loop(buffer, new_line);
    next_line = ft_handle_nl(buffer, new_line);
    retun (next_line);
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

#### 2.d - A próxima linha

