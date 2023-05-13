# Enunciado

Implemente um servidor multithreaded utilizando a linguagem C. Este servidor deve instanciar um conjunto de n threads trabalhadoras, onde n é uma constante do programa. Durante a instanciação, cada thread trabalhadora deve receber um número que a identifica de forma única. Adicionalmente, o servidor deve instanciar uma thread dispatcher que ira receber as requisições, atribuindo cada nova requisição a uma thread trabalhadora que esteja disponível.

A chegada das requisições serão simuladas através de um arquivo texto, onde cada linha corresponde a uma requisição contendo os parâmetros necessários para a invocação do método provido pelo servidor separados por ponto e vírgula. A thread dispatcher deve ler o arquivo texto linha a linha em uma periodicidade expressa em milisegundos e também definida por uma constante temporeq.

Para simularmos um tempo de execução da requisição no servidor, o método por ele disponibilizado deve receber dois parâmetros: i e tempoespera. O parâmetro i informa a quantidade de dígitos do número pi que deve ser calculado pelo servidor. O parâmetro tempoespera expressa um tempo em milisegundos pelo qual o método deve esperar até imprimir em um arquivo de saída um número sequencial correspondente a quantidade de requisições já processadas pela thread, os parâmetros recebidos na requisição e o valor de pi calculado. Cada linha do arquivo deve corresponder a uma requisição processada e estes valores devem estar separados por ponto e vírgula. O arquivo de saída deve ter como nome o identificador da thread acrescido de ¨.txt¨.

O valor do parâmetro i deve variar de forma aleatória de 10 a 100, enquanto tempoespera deve variar de forma aleatória entre os valores 500 e 1500 nas requisições a serem geradas e armazenadas no arquivo texto de requisições. O programa principal deve receber a quantidade de requisições a serem geradas como parâmetro de sua execução. O programa principal, então, irá gerar o arquivo de requisições, instanciará as threads trabalhadoras e, finalmente, a thread dispathcher que iniciará o processamento das requisições.

## Programa principal

- recebe: quantidade de requisições a serem geradas
- constantes:
  - n: número de threads trabalhadoras
  - temporeq: periodicidade das requisições em milisegundos
- execução:
  - gera o arquivo de requisições
  - instancia as threads trabalhadoras
  - instancia a thread dispatcher
    - processa as requisições
- arquivos gerados:
  - arquivo de requisições: requisicoes.txt
    - cada linha: i;tempoespera
  - arquivo de saída da thread: identificador_da_thread.txt
    - cada linha: requisicoes_processadas;i;tempoespera;valor_de_pi

## Funções

- int get_aleatorio(int min, int max)
- char* get_pi(char* destino, int digitos)
- void gera_requisicoes(char* nome_arquivo, int quantidade_requisicoes, int digitos_min, int digitos_max, int tempo_min, int tempo_max)

## Threads

- n threads trabalhadoras com número de identificação único
- 1 thread dispatcher
  - "recebe" as requisições, atribuindo cada uma para uma thread trabalhadora disponível
  - deve ler o arquivo texto (simulação de requisições) com uma periodicidade em milissegundos (temporeq)

## Arquivo de requisições

- simuladas por um arquivo texto
- cada linha contém dois parâmetros separados por ponto-e-vírgula: i e tempoespera
  - i (aleatório de 10 a 100) informa a quantidade de dígitos do número pi que deve ser calculado pelo servidor
  - tempoespera (aleatório de 500 a 1500) expressa um tempo em milisegundos pelo qual o método deve esperar até imprimir em um arquivo de saída um número sequencial correspondente a quantidade de requisições já processadas pela thread, os parâmetros recebidos na requisição e o valor de pi calculado. Cada linha do arquivo deve corresponder a uma requisição processada e estes valores devem estar separados por ponto e vírgula. O arquivo de saída deve ter como nome o identificador da thread acrescido de ¨.txt¨.

