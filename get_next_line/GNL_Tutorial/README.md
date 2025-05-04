# Episódio 1: A teoria da Get_Next_Line
## 1: Entendendo seu projeto:

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


## 4_Next

Bem vindos ao pesadelo (=

Antes de começarmos, vamos ver porquê essa sessão é necessária: experimente rodar o programa broken_next.c encontrado na pasta 4_Next.

Repare que o output do programa é quebrado: a primeira linha volta exatamente como queremos, todo texto até a primeira ocorrência de um \n, depois nada, perfeito!

Mas a segunda chamada da função retorna uma linha com algumas letras perdidas no começo.

Isso está acontecendo porquê, como estabelecemos lá atrás na parte **1 - Entendendo seu projeto:", a função _read()_ é esperta e lembra o que já leu!

Como estamos lendo BUFFER_SIZE bytes do fd (caso você não declare um novo BUFFER_SIZE, o padrão em todos esses programas é 42), a segunda vez que chamamos a função, _read()_ vai continuar a ler de onde parou. Se havia texto depois da primeira \n, isso vai ser jogado fora!

Agora é hora de revelar nossa arma secreta para esse projeto: variáveis estáticas!

Experimente com o programa statics.c, também localizado na 4_Next. Entendeu o que está acontecendo aqui?

Uma variável estática, ao contrário de uma variável local comum, _só para de existir quando o programa que a criou termina de rodar._ Toda vez que o programa statics.c chama a função count_local(), ele está criando uma **nova** variável local chamada i, guardando o número 0 nela, adicionando 1 ao 0 e retornando esse total. Quando a função count_local() retorna, i é apagada, destruída, assassinada, joga contra a Alemãnha numa copa em seu país natal etc..

Porém, a variável i que existe dentro da função count_static é diferente, especial: ela lembra que valor tinha quando foi chamada da última vez, e só vai ser apagada quando o programa terminar de rodar!

Além disso, se você brincou com os comentários na declaração de i dentro da count_static() já deve saber disso, mas variáveis estáticas, ao contrário das locais, _são iniciadas com valor determinado!_ Caso você não declare nada, o computador vai presumir que você queria um 0 salvo nela, portanto, não há diferença entre criar "static int i = 0" ou "static int i", ambas são iguais!

É aqui que, pela primeira vez, irei abandonar completamente o uso de ponteiros para o buffer, pois precisamos falar das diferentes memórias que o computador possuí...

### Memories...
![illinois grainger CS 225](https://courses.grainger.illinois.edu/cs225/fa2022/assets/notes/stack_heap_memory/memory_layout.png)

O confundidor diagrama acima é a versão MENOS confusa que achei disso xD

Nossos programas recebem de presente do PC acesso à memória RAM disponível quando são executados. C, sendo uma língua responsável, tem um complexo método de trabalhar com essa memória: ele guarda variáveis locais (como a int i da count_local!) na parte chamada stack. Dentro do heap está o que nós usamos desde o C06: memória alocada com malloc, calloc etc..

Isso quer dizer que desde o C00 na piscine nós estávamos usando o stack. Quando criamos o nosso ft_putchar(char c), quer adivinhar onde o dito "char c" foi parar na memória do coputador? No stack! E desde o C06 passamos a usar o heap, onde todos nossos mallocs foram parar.

Sabe como **todo maldito malloc que fazemos** precisa ser seguido por uma linha de código dizendo

```
arr = malloc(10);
if (!arr)
    return (NULL);
```

? Olhando para esse gráfico acima, você pode descobrir o motivo disso! Memória usada no heap cresce de baixo para cima, e no stack é o oposto, de cima para baixo. Isso quer dizer que, toda vez que usamos malloc, há uma _chance_ das duas colidirem: e se seu programa tem 100bytes de memória disponíveis sobrando entre o stack e o heap, e você tenta fazer um malloc(101)? O seu programa vai ver que está prestes a misturar o heap com o stack e dizer: NOPE e negar seu malloc, por isso precisamos sempre ter em mente que "talvez um malloc dê errado!"

E é nesse ponto que entra meu profundo desacordo com os que querem fazer GNL com ponteiros: assim como C00 introduziu o stack e C06 introduziu o heap, GNL é nosso momento de finalmente podermos aprender sobre o próximo bloco de memória: Data!

#### Data

Apenas 2 tipos de coisas são salvas no bloco Data: variáveis globais e variáveis estáticas. Porém, bem, a Norma nos proíbe de jamais sonhar em usar variáveis globais, então nossa única chance de brincar com as estranhas propriedades desse bloco de memória é com variáveis estáticas, que são introduzidas no GNL.

Você **pode** criar um `static char *buffer` e depois fazer um `buffer = malloc(BUFFER_SIZE + 1)`, mas fazer isso é jogar fora toda chance de aprender sobre estáticas nesse exercício!

Tudo que você está fazendo se criar um static char * é dizer "PC, guarde no bloco de memória data o número de um endereço de memória no heap. Depois disso, foda-se data, vou continuar o resto do trabalho como se esse fosse um simples projeto trabalhando com memória heap, tchau."

Além de garantir que é fisicamente impossível seu GNL não ter memory leaks, você está abandonando todo o propósito do projeto: aprender a usar a memória data!

#### Pra quê usar a Data?

Os blocos de memória Unitialized Data e Initialized Data (que são basicamente iguais, Unitialized é só onde vão parar globais e estáticas que você não dá um valor na hora de criar) são especiais. Como você já viu ao usar o statics.c, dados salvos em data são mantidos do começo ao fim do programa, mesmo que a função que os criou termine de ser executada.

Essa memória também é tratada de forma igual à memória do stack, onde variáveis locais vão parar: você não precisa se preocupar com alocar e depois dar free em nada que coloca na data: o próprio programa vai fazer isso para você, como sua última ação quando estiver acabando de rodar.

Um outro motivo, bem arcáico e meio obsoleto para se considerar usar a data não é um fator há vários anos, mas vou dizê-lo ainda assim, por motivos históricos: perceba que os blocos de memória são descritos indo de "low address" (endereços baixos) para "high adress" (endereços altos).

Existe uma diferença _física_ entre onde dados salvos em data, heap e stack vão parar. Se seu PC quer acessar dados no stack, precisa "viajar" mais longe na memória para achá-los. Se vai buscar no heap, é um pouco mais perto. Na data, é o mais rápido que jamais será: apenas o próprio código (que fica salvo no bloco de memória text) é acessível mais rápido pro PC do que dados salvos em data.

Hoje em dia, com nossos processadores de bilhões de operações por segundo e mais memória do que jamais vamos precisar, isso não importa muito, mas antigamente programas que usavam mais data do que heap ou stack eram mais velozes, apenas porquê a máquina precisava "viajar" menos pra acessar os dados!

### De volta ao "Next"

Bem, agora temos o último ingrediente da nossa receita para uma GNL perfeita:
`static char buffer[BUFFER_SIZE]`

Com isso do nosso lado, nós podemos eficientemente guardar o que "sobrou" da última vez que devolvemos uma linha e adicionar isso à próxima linha que criarmos quando a função for chamada de novo antes de começarmos a ler mais coisas do arquivo, garantindo que não vamos perder letras!

## 5 Get Next Line

Este é o fim da parte teórica crianças, hora de botar a mão na massa!
Eu recomendaria, do fundo de meu coração, que você tirasse esse momento para tentar escrever sua GNL. Desenhe o diagrama do código, faça notas do que vai precisar, pergunte ao chat GPT o que ele acha...

Por favor, _tente_ fazer ela com base apenas nessa parte teórica, pois eu vou estar cobrindo o código num passo à passo muito minuncioso de como minha lógica funciona à seguir, e não quero que você seja roubado da chance de criar seu próprio código e acabe copiando o meu.

Caso queira continuar para o passo à passo, acesse a pasta 5_get_next_line, onde o tutorial vai continuar, cobrindo em detalhe como construír sua GNL do começo ao fim!

# Bonus: ponteiros vs array

Bem, eu tinha que falar disso...

A impressão que eu tenho é que, anos atrás, quando get_next_line foi adicionado na lista de projetos da 42, alguém escreveu um tutorial em Francês. O criador desse tutorial não percebeu que havia criado uma GNL que sempre faz memory leaks: uma GNL com ponteiros em vez de array. E desde lá, pessoas sempre copiam disso, numa bola de neve sem fim.

Em minha opinião, é inquestionável que fazer GNL com ponteiros é um projeto com memory leak. A questão é: a Moulinette não faz um teste de valgrind na get_next_line. Então nós, como avaliadores, deveríamos dar 0 por memory leak nesse projeto, como somos ordenados a sempre fazer, ou considerar que "hey, a Moulinette não pega esse erro, vamos deixar só esse projeto passar com memory leak"?

Pessoalmente, eu fico no segundo campo. Existem condições de dar 0 em avaliações que devemos ter controle sempre, como verificar cheats ou se o aluno não sabe seu próprio código. Porém, memory leaks _deveria_ ser algo fácil da Moulinette testar, então fica a questão: se ela não testa, é pra gente testar?

Para exemplificar quão insanamente fácil seria testar isso, heis o meu tester para saber se uma GNL vaza memória por ter sido feita com ponteiros, salvo em um arquivo chamado main.c:
```
#include "get_next_line.h"
#include <fcntl.h>

int    main(void)
{
    int     fd;
    char    *line;

    fd = open("main.c", O_RDONLY);
    line = get_next_line(fd);
    free(line);
    close(fd);
    return (0);
}
```
Com isso, basta ter um arquivo chamado tester.sh com o seguinte código:
```
cmp <(echo "All") <(valgrind --log-fd=1 ./a.out 2>&1 | awk '/were freed|leaked memory/ {print $2}')
```
E pronto, esse sh vai retornar nada se sua GNL não tem vazamento de memória, ou "/proc/self/fd/11 /proc/self/fd/17 differ: byte 1, line 1" se houve memory leak.

Casando quão estupidamente simples esse teste é, literalmente uma main.c com apenas 8 linhas (e isso porquê eu quis fazer ela pela Norma!) e um .sh com uma única linha, com o fato de testes como esse serem comuns na Moulinette, me faz questionar: a 42 _sabe_ que GNL com ponteiros vazam memória, e decidiu que isso não importa? Porquê nada além de "a gente decidiu deixar esse passar" pode explicar o motivo de um teste tão pequeno não ser feito.

Por isso, meu voto é: não, não devemos dar 0 com causa de "Memory Leak" para pessoas que escrevem o GNL com ponteiros.

Agora, se me permitem me indulgir um pouco, vem a parte contraditória: apesar de considerar que, para os propósitos do projeto conforme aparece na Milestone 1, podemos deixar passar, eu considero, sem sombra de dúvida alguma, que escrever seu GNL com ponteiros em vez de array é inquestionavelmente _errado._

## Motivo 1: Data
Como discuti acima na parte teórica, um dos intuitos da GNL é nos familiarizar com variáveis estáticas. Criar um ponteiro estático que aponta para um endereço de memória do heap _não é usar variáveis estáticas._ Ao fazer isso, você está misturando os dois tipos de memória, num ato que está _implorando_ para causar problemas (problemas como o inevitável vazamento de memória de BUFFER_SIZE + 1).

Além disso, ver um projeto feito com ponteiros me faz questionar: quão a sério esse aluno levou o estudo das static variables que o subject da GNL recomendou que fizéssemos?

Pela primeira vez na 42, esse projeto nos convida a estudar sobre os diferentes tipos de memória do computador. Após ter criado a minha GNL, eu me sinto feliz de achar que entendo várias coisas que antes não faziam nenhum sentido para mim: "por quê eu não consigo compilar se escrever algo como `char arr[7]; arr = "Hello!;"`?" "Por quê eu preciso dar free()?" "Por quê diabos o computador _não_ me impede de escrever algo como `int *p; p = (int *)500;`, isso é loucura!"

Claro, algumas (talvez todas) questões podiam/deviam ter sido consideradas lá atrás no C06. Talvez você já as tenha ponderado, não sei. Mas para mim, GNL acabou sendo um momento de aprendizado incrível... Que eu podia ter jogado completamente fora criando um ponteiro estático para o heap.

## Motivo 2: get_NEXT_line, não get_ALL_lineS

Um argumento comum dos que acham que GNL deve ser sempre feita com ponteiros é que "hey, se o usuário usar minha GNL para ler todas linhas de um arquivo do começo ao fim, não há memory leaks!"

Para mim, esse é um argumento muito fraco, considerando que o propósito da função está no nome dela: nossa GNL deve ser capaz de retornar uma linha, duas, três, o arquivo inteiro, o usuário que vai escolher isso. Se temos a obrigação de criar nosso código sem vazamento de memória, os usos da GNL fora do escopo de "loop para ler arquivo inteiro" devem ser considerados.

Considere a seguinte, 100% insana aplicação da função ft_calloc:
```
void    *ft_calloc(size_t n, size_t size)
{
	void    *p;
    void    *leak;
	ptr = malloc(n * size);
	if (!p)
		return (p);
	ft_bzero(p, size * n);
    if (size * n == 42)
        leak = malloc(100);
	return (p);
}
```
Reparou aquele malloc desnecessário, toda vez que n * size é igual à 42? Esse malloc vai causar um memory leak toda vez que essa ft_calloc for chamada para alocar exatamente 42 bytes de memória... Devemos considerar que esse código não tem nenhum memory leak? Essa ft_calloc _ainda_ funciona, sempre retorna um ponteiro com o tamanho que o usuário quer: ela está ou não com defeito?

Agora considere: o que uma GNL com ponteiros faz é o OPOSTO disso! Em vez de haver um caso extremo no qual ela não causa memory leaks, na verdade ela vai causar leaks em _todos possíveis usos desejados,_ exceto um caso específico.

Então seria mais como se a condição desnecessária fosse `if (size *n != 42)`!

## Motivo 3: "qualquer BUFFER_SIZE"

Outra justificativa comum de se encontrar é que "hey, o meu GNL não dá memory leak se o usuário rodar ele com BUFFER_SIZE=1! Se você rodar com BUFFER_SIZE=2 ou mais, é culpa sua!"

Assim como a anterior, em minha opinião, essa desculpa é bastante fraca. Somos claramente instruídos a criar nosso GNL para funcionar com qualquer BUFFER_SIZE positivo pedido pelo usuário.

Eu estou mais que disposto a adimitir que BUFFER_SIZEs maiores do que SIZE_MAX não são nosso trabalho: o usuário está esperando receber uma string, se ele escolheu como buffer um valor maior do que é possível escrever em uma string, isso _é_ culpa dele. Porém argumentar que o usuário tem que saber que vai perder memória se não disser um BUFFER_SIZE de 1 é outra história.

"Eu vou colocar na documentação" é a versão mais impressionante desse argumento. Eu concordo que, se você estivesse criando uma biblioteca para uso em C, e escrevesse na documentação da sua GNL "hey, cuidado: essa função vaza memória se você não chegar ao EOF do fd. Se quer apenas 1 ou 2 linhas, lembre de fazer um loop
```
while (line)
{
    free(line);
    line = get_next_line(fd);
}
```
ou aceite que vai perder memória. Caso não queira perder tempo fazendo isso, por favor, certifique-se de sempre rodar a função com BUFFER_SIZE = 1" seria uma solução _completamente_ aceitável.

Porém, infelizmente, nós não temos o luxo de escolher a documentação de nossa GNL durante a Milestone 1. O mais perto que chegamos de ter uma documentação para ela é o subject, e o subject não diz nada sobre haver um aceitável vazamento de memória quando BUFFER_SIZE != 1.

Para mim, argumentar que "se for 1 não dá problemas" é o mesmo que argumentar que esta função é uma perfeita aplicação de ft_strlen:
```
size_t    ft_strlen(char *str)
{
    return (42);
}
```
Basta argumentar que "hey, desde que o usuário tenha usado essa strlen de forma correta e tenha enviado uma string que tinha 42 caractéres, minha função pega! Okay, o resultado dela dá errado para qualquer outro tamanho de string, mas existe um caso entre 0 e SIZE_MAX no qual ela dá o resultado correto, então é uma strlen perfeita!"

## Motivo 4: _não_ é mais difícil

Um argumento que me deixa triste por ser tão comum é que "eu vou fazer meu GNL com ponteiro em vez de array, fazer com array é confuso demais, nada faz sentido!"

Esse argumento me deixa triste porquê eu sei que parte do embasamento dele é culpa minha. Existem mil tutoriais na internet ensinando a fazer o GNL com ponteiros, e mais mil que ensinam a fazer com linked lists (que no fundo quer dizer "linked list com um content de tipo ponteiro", ou seja, dá na mesma). Eu sei, eu li esses tutoriais quando estava escrevendo a minha, e meu deus, eles foram _vitais._

Pessoas com muito melhor conhecimento de código e de como explicar código do que eu já fizeram isso, e eu _sei_ quão fácil é seguir os passos... E aí você tem meu código, guardado na pasta GNL_0 desse repositório: uma aberração.

Se você viajasse no tempo para 3 semanas atrás e me oferecesse uma escolha: tentar entender o meu código no GNL_0 ou seguir um tutorial bem escrito de GNL com ponteiros, nem fodendo que eu escolheria a primeira opção.

A questão é: o GNL salvo em GNL_0 foi produto de duas semanas de insanidade, de reparos feitos em cima de reparos feitos em cima de patches para edgecases em cima de recomendações idióticas do Chat GPT em cima de pedaços de tutoriais na internet. Eu não vou sequer começar a fingir que aquela GNL está bem escrita. Isso não quer dizer que existe uma dificuldade inerênte em escrever seu GNL com array em vez de ponteiros.

Se eu quisesse, eu poderia trocar o meu GNL para uma versão de ponteiro em vez de array, bastaria trocar `static char buffer[BUFFER_SIZE + 1]` por 
```
static char    *buffer;

if (!buffer)
{
    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
}
```
E o resto do código inteiro seria literalmente igual. Me diga, honestamente, que essa modificação no código que se encontra na GNL_0 faria com que ele miraculosamente ficasse fácil de se ler e fizesse perfeito sentido.

99% da minha motivação para passar os últimos 2 dias escrevendo esse tutorial veio da culpa que eu sinto por minha parte nesse mito. Eu espero ter escrito a lógica e o passo à passo de forma suficientemente clara para mostar que não, não é mais fácil escrever GNL com ponteiros do que com arrays. O oposto também não é verdade. Ambos métodos são tão difíceis quanto o outro, nada no desafio da GNL tem nada a ver com se seu buffer (ou leftovers ou vault ou o que quiser chamar) é um ponteiro ou um array.

Vou encerrar por aqui. Caso tenha algo que queira conversar sobre o assunto, ou caso tenha se sentido perdido tentando ler esse tutorial, achar que algo que eu expliquei ficou mal explicado, o que for, não hesite em me contactar no slack, thenriqu , ou pode me procurar no Cluster 1, eu semrpe estou na fileira 12 ou na 13, sempre colado na parede!

Abraços à todos, obrigado por perder tanto tempo de sua vida lendo tudo isso, e keep swimming!
