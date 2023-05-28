#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

// valor de PI com precisão de 100 casas decimais
const char PI[] = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";

// tipo booleano
typedef enum bool
{
  false = 0,
  true = 1
} bool;

int main(const int, const char **);
int read_quantidade_requisicoes(const int, const char **);
bool gera_requisicoes(const char *, const int, const int, const int, const int, const int);

char *get_pi(char *, const int);

// funções auxiliares
int get_aleatorio(const int, const int);

// função principal
int main(const int argc, const char **argv)
{
  // número de threads trabalhadoras
  const int N = 10;

  // periodicidade das requisições em milisegundos
  const int TEMPOREQ = 250;

  // valores dos intervalos
  const int digitos_min = 10;
  const int digitos_max = 100;
  const int tempoespera_min = 500;
  const int tempoespera_max = 1500;

  // nome do arquivo de requisições
  const char arquivo_requisicoes[] = "requisicoes.txt";

  /**
   * lê a quantidade de requisições do
   * parâmetro passado ao programa ou do input
   */
  int quantidade_requisicoes = read_quantidade_requisicoes(argc, argv);

  // gera o arquivo de requisições
  bool requisicoes = gera_requisicoes(
      arquivo_requisicoes,
      quantidade_requisicoes,
      digitos_min,
      digitos_max,
      tempoespera_min,
      tempoespera_max);

  // verifica se o arquivo foi ou não gerado com sucesso
  if (!requisicoes)
  {
    puts("Erro ao tentar criar o arquivo de requisições!");
    return 0;
  }

  // TODO: Instanciar threads trabalhadoras
  // TODO: Instanciar thread dispatcher

  return 0;
}

/**
 * Lê a quantidade de requisições dos argumentos passados ao programa ou,
 * caso não tenha sido passado nenhum argumento, lê do input do usuário.
 */
int read_quantidade_requisicoes(const int argc, const char **argv)
{
  int quantidade_requisicoes = 0;

  // checa se foi passado argumentos para o programa
  if (argc > 1)
  {
    // lê o primeiro argumento e avisa caso seja inválido
    if (sscanf(argv[1], "%d", &quantidade_requisicoes) != 1)
    {
      puts("Argumento inválido.");
    }
  }
  else
  {
    puts("Nenhum argumento foi passado ao programa.");
  }

  while (quantidade_requisicoes <= 0)
  {
    char entrada[32];

    printf("%s", "Digite a quantidade de requisições a serem geradas:\n> ");

    // lê uma entrada do usuário
    fgets(entrada, sizeof(entrada), stdin);

    // se a entrada for inválida ou a quantidade for 0, alerta usuário e pede uma nova entrada
    if (sscanf(entrada, "%d", &quantidade_requisicoes) != 1 || quantidade_requisicoes <= 0)
    {
      puts("Entrada inválida, por favor tente novamente.");
    }
  }

  return quantidade_requisicoes;
}

/**
 * Gera o arquivo de requisições e retorna true em caso de sucesso e
 * false em caso de erro
 */
bool gera_requisicoes(
    const char *nome_arquivo,
    const int quantidade_requisicoes,
    const int digitos_min,
    const int digitos_max,
    const int tempo_min,
    const int tempo_max)
{
  char *txt = calloc(1, sizeof(char));
  if (txt == NULL)
  {
    puts("Erro ao alocar memória para o texto das requisições!");

    // retorna erro
    return false;
  }

  // inicializa seed de números aleatório com o tempo atual
  srand(time(NULL));

  int digitos, tempo;

  // quantidade de caracteres que um inteiro pode ter
  unsigned long tamanho_int;
  char str[21];
  sprintf(str, "%d", INT_MAX);
  tamanho_int = strlen(str);

  // cria o texto
  for (int i = 0; i < quantidade_requisicoes; i++)
  {
    digitos = get_aleatorio(digitos_min, digitos_max);
    tempo = get_aleatorio(tempo_min, tempo_max);

    // tamanho do texto
    unsigned long int tamanho_antigo = strlen(txt) * sizeof(char);

    // tamanho da linha: int + ';' + int + '\n'
    unsigned long int tamanho_da_linha = tamanho_int + sizeof(char) + tamanho_int + sizeof(char);

    // tamanho antigo + tamanho da linha
    txt = realloc(txt, tamanho_antigo + tamanho_da_linha);

    // linha: tamanho da linha + '\0'
    char linha[tamanho_da_linha + sizeof(char)];
    sprintf(linha, "%d;%d\n", digitos, tempo);

    strcat(txt, linha);
  }

  // remove último \n
  char *ultimo_nl = strrchr(txt, '\n');
  if (ultimo_nl != NULL)
  {
    *ultimo_nl = '\0';
  }

  // cria arquivo
  FILE *fp = fopen(nome_arquivo, "w");
  if (fp == NULL)
  {
    printf("Erro ao criar arquivo %s!\n", nome_arquivo);

    // retorna erro
    return false;
  }

  // escreve no arquivo
  fprintf(fp, "%s", txt);
  fclose(fp);

  free(txt);

  // retorna sucesso
  return true;
}

/**
 * Retorna um número aleatório no intervalo [min, max].
 */
int get_aleatorio(const int min, const int max)
{
  int min_cpy = min, max_cpy = max;
  // corrige se ordem estiver invertida
  if (max_cpy < min_cpy)
  {
    int temp = min_cpy;
    min_cpy = max_cpy;
    max_cpy = temp;
  }
  int numero_aleatorio = rand() % (max_cpy - min_cpy + 1) + min_cpy;
  return numero_aleatorio;
}

/**
 * Coloca PI com a quantidade especificada de dígitos em destino e retorna seu endereço.
 */
char *get_pi(char *destino, const int digitos)
{
  int quantidade_caracteres;
  char *str;

  if (digitos <= 0)
  {
    quantidade_caracteres = 1;
    str = "3";
  }
  else
  {
    // '3' + '.' + quantidade de digitos
    quantidade_caracteres = 2 + digitos;
    strcat(str, PI);
  }

  strncpy(destino, str, quantidade_caracteres);
  destino[quantidade_caracteres] = '\0';

  return destino;
}
