#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]);
int get_aleatorio(int, int);

int main(int argc, char const *argv[])
{
  // inicializa seed de números aleatório com o tempo atual
  srand(time(NULL));

  for (int i = 0; i < 10; i++)
  {
    printf("%d: %d\n", i, get_aleatorio(6, 9));
  }
  return 0;
}

/**
 * Retorna um número aleatório no intervalo [min, max].
 */
int get_aleatorio(int min, int max)
{
  int numero_aleatorio = rand() % (max - min + 1) + min;

  return numero_aleatorio;
}