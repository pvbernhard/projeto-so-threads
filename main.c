#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// valor de PI com precisão de 100 casas decimais
const char PI[] = "3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";

int main(int, char **);
int read_quantidade_requisicoes(int, char **);
void gera_requisicoes(char *, int, int, int, int, int);
int get_aleatorio(int, int);
char *get_pi(char *, int);

int main(int argc, char **argv)
{
  // número de threads trabalhadoras
  const int N = 10;

  // periodicidade das requisições em milisegundos
  const int TEMPOREQ = 250;

  int quantidade_requisicoes = read_quantidade_requisicoes(argc, argv);

  // TODO: Gerar arquivo de requisições
  // TODO: Instanciar threads trabalhadoras
  // TODO: Instanciar thread dispatcher

  return 0;
}

int read_quantidade_requisicoes(int argc, char **argv)
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

  while (quantidade_requisicoes == 0)
  {
    char entrada[256];

    printf("%s", "Digite a quantidade de requisições a serem geradas:\n> ");

    // lê uma entrada do usuário
    fgets(entrada, sizeof(entrada), stdin);

    // se a entrada for inválida ou a quantidade for 0, alerta usuário e pede uma nova entrada
    if (sscanf(entrada, "%d", &quantidade_requisicoes) != 1 || quantidade_requisicoes == 0)
    {
      puts("Entrada inválida, por favor tente novamente.");
    }
  }

  return quantidade_requisicoes;
}

void gera_requisicoes(
    char *nome_arquivo,
    int quantidade_requisicoes,
    int digitos_min,
    int digitos_max,
    int tempo_min,
    int tempo_max)
{
  // TODO
}

/**
 * Retorna um número aleatório no intervalo [min, max].
 */
int get_aleatorio(int min, int max)
{
  int numero_aleatorio = rand() % (max - min + 1) + min;
  return numero_aleatorio;
}

/**
 * Coloca PI com a quantidade especificada de dígitos em destino e retorna seu endereço.
 */
char *get_pi(char *destino, int digitos)
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
