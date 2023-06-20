#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <gmp.h>

#include <string.h>
#include <time.h>
#include <limits.h>

// TIPOS

typedef enum bool
{
  false = 0,
  true = 1
} bool;

typedef enum erros
{
  erro = 1,
  sucesso = 0
} erros;

typedef struct Requisicao
{
  int digitos_pi;
  int tempo_espera;
} Requisicao;

typedef struct ParametrosDispatcher
{
  int n_workers;
  int tempo_req;
  const char *arquivo_requisicoes;
} ParametrosDispatcher;

// VARIÁVEIS GLOBAIS

pthread_mutex_t mutex_workers;
pthread_cond_t cond_workers;

int workers_disponiveis = 0;

pthread_mutex_t mutex_req;

Requisicao requisicao_a_fazer;

// FUNÇÕES

int main(const int, const char **);

void *thread_worker_func(void *);
void *thread_dispatcher_func(void *);

int read_quantidade_requisicoes(const int, const char **);
bool gera_requisicoes(const char *, const int, const int, const int, const int, const int);

char *get_pi(char *, const int);

// funções auxiliares
int get_aleatorio(const int, const int);
unsigned long get_tamanho_caracteres_int();

// função principal
int main(const int argc, const char **argv)
{
  // int digitos = 1000;
  // // memória: inteiro (3) + ponto + digitos + \0
  // char *pi = (char *)calloc(2 + digitos + 1, sizeof(char));
  // get_pi(pi, digitos);

  // printf("PI: %s\n", pi);
  // return 0;

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
  const int quantidade_requisicoes = read_quantidade_requisicoes(argc, argv);

  // gera o arquivo de requisições
  erros requisicoes = gera_requisicoes(
      arquivo_requisicoes,
      quantidade_requisicoes,
      digitos_min,
      digitos_max,
      tempoespera_min,
      tempoespera_max);

  // verifica se o arquivo foi ou não gerado com sucesso
  if (requisicoes == erro)
  {
    puts("Erro ao tentar criar o arquivo de requisições!");
    return 0;
  }

  pthread_t thread_dispatcher;
  ParametrosDispatcher args;

  args.n_workers = N;
  args.tempo_req = TEMPOREQ;
  args.arquivo_requisicoes = arquivo_requisicoes;

  if (pthread_create(&thread_dispatcher, NULL, thread_dispatcher_func, (void *)&args) != 0)
  {
    printf("Erro ao criar a thread\n");
    return erro;
  }

  // TODO: Instanciar threads trabalhadoras
  // TODO: Instanciar thread dispatcher

  // aguarda a thread terminar
  pthread_join(thread_dispatcher, NULL);

  return sucesso;
}

void *thread_worker_func(void *args)
{
  printf("WORKER!\n");
  pthread_exit(NULL);
}

void *thread_dispatcher_func(void *args)
{
  // lê parâmetros
  ParametrosDispatcher *arg = (ParametrosDispatcher *)args;
  const int N_WORKERS = arg->n_workers;
  const int TEMPOREQ = arg->tempo_req;
  const char *ARQUIVO = arg->arquivo_requisicoes;

  // inicializa workers
  pthread_t thread_worker[N_WORKERS];
  for (int i = 0; i < N_WORKERS; i++)
  {
    pthread_create(&thread_worker[i], NULL, thread_worker_func, NULL);
  }

  // inicializa mutexes
  pthread_mutex_init(&mutex_workers, NULL);
  pthread_cond_init(&cond_workers, NULL);
  pthread_mutex_init(&mutex_req, NULL);

  unsigned long tamanho_int = get_tamanho_caracteres_int();
  // memória: int + ';' + int + '\n' + '\0'
  char *linha = (char *)calloc(tamanho_int + 1 + tamanho_int + 2, sizeof(char));

  FILE *fp = fopen(ARQUIVO, "r");
  if (fp == NULL)
  {
    printf("Erro ao ler arquivo %s!\n", ARQUIVO);
    return NULL;
  }

  int digitos_pi;
  int tempo_espera;
  int n_scan;

  // lê o arquivo linha por linha
  while (fgets(linha, sizeof(linha), fp) != NULL)
  {
    // printf("%s", linha);

    pthread_mutex_lock(&mutex_req);

    n_scan = sscanf(
        linha,
        "%d;%d",
        &(requisicao_a_fazer.digitos_pi),
        &(requisicao_a_fazer.tempo_espera));

    if (n_scan == 2)
    {
      pthread_mutex_unlock(&mutex_req);
      // aguarda o intervalo
      usleep(TEMPOREQ * 1000);
    }
    else
    {
      pthread_mutex_unlock(&mutex_req);
    }
  }

  fclose(fp);
  free(linha);

  // espera workers finalizarem
  for (int i = 0; i < N_WORKERS; i++)
  {
    pthread_join(thread_worker[i], NULL);
  }

  pthread_mutex_destroy(&mutex_workers);
  pthread_cond_destroy(&cond_workers);
  pthread_mutex_destroy(&mutex_req);

  pthread_exit(NULL);
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
    return erro;
  }

  // inicializa seed de números aleatório com o tempo atual
  srand(time(NULL));

  int digitos, tempo;

  // quantidade de caracteres que um inteiro pode ter
  unsigned long tamanho_int = get_tamanho_caracteres_int();

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
    return erro;
  }

  // escreve no arquivo
  fprintf(fp, "%s", txt);
  fclose(fp);

  free(txt);

  // retorna sucesso
  return sucesso;
}

/**
 * Retorna um número aleatório no intervalo [min, max]
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
 * Calcula o valor de PI com o algoritmo de Gauss–Legendre, coloca o valor
 * com a quantidade especificada de dígitos em destino e retorna seu endereço
 */
char *get_pi(char *destino, const int digitos)
{
  // bits para a precisão de dígitos - 2 dígitos a mais por garantia
  int precisao = (digitos + 2) * 3.35; // log2(10) == 3.32

  /**
   * calcula quantas iterações são necessárias.
   * 1 iteração: 2 dígitos
   * 2 iterações: 7 dígitos
   * 3 iterações: 7*2=14 dígitos (18, na realidade)
   * ...
   */
  int iteracoes;
  if (digitos <= 2)
  {
    iteracoes = 1;
  }
  else
  {
    iteracoes = 2;
    int max_digitos = 7;
    while (max_digitos < digitos)
    {
      iteracoes++;
      max_digitos *= 2;
    }
  }

  // inicializa variáveis do algoritmo
  mpf_t a_0, a_n, b_0, b_n, t_0, t_n, p_0, p_n, pi;

  mpf_init2(a_0, precisao);
  mpf_init2(b_0, precisao);
  mpf_init2(t_0, precisao);
  mpf_init2(p_0, precisao);

  mpf_init2(a_n, precisao);
  mpf_init2(b_n, precisao);
  mpf_init2(t_n, precisao);
  mpf_init2(p_n, precisao);

  mpf_init2(pi, precisao);

  /** 1 */
  mpf_set_ui(a_0, 1); // a0=1
  /** 1/sqrt(2) */
  mpf_set_ui(b_0, 2);      // b0=2
  mpf_sqrt(b_0, b_0);      // b0=sqrt(b0)
  mpf_ui_div(b_0, 1, b_0); // bo=1/b0
  /** 1/4 */
  mpf_set_ui(t_0, 1);      // to=1
  mpf_div_ui(t_0, t_0, 4); // t0=t0/4
  /** 1 */
  mpf_set_ui(p_0, 1); // p0=1

  for (int i = 0; i < iteracoes; i++)
  {
    /** an=(a0+b0)/2 */
    mpf_add(a_n, a_0, b_0);  // an=a0+b0
    mpf_div_ui(a_n, a_n, 2); // an=an/2
    /** bn=sqrt(a0*b0) */
    mpf_mul(b_n, a_0, b_0); // bn=a0*b0
    mpf_sqrt(b_n, b_n);     // bn=sqrt(bn)
    /** tn=t0-p0*(a0-an)^2 */
    mpf_sub(t_n, a_0, a_n);  // tn=a0-an
    mpf_pow_ui(t_n, t_n, 2); // tn=tn^2
    mpf_mul(t_n, p_0, t_n);  // tn=p0*tn
    mpf_sub(t_n, t_0, t_n);  // tn=t0-tn
    /** pn=2*p0 */
    mpf_mul_ui(p_n, p_0, 2); // pn=p0*2

    // atualiza variáveis anteriores (*0) para valores atuais (*n)
    mpf_set(a_0, a_n);
    mpf_set(b_0, b_n);
    mpf_set(t_0, t_n);
    mpf_set(p_0, p_n);
  }

  /** pi=[(an+bn)^2]/(4*tn) */
  mpf_add(pi, a_n, b_n); // pi=an+bn
  mpf_pow_ui(pi, pi, 2); // pi=pi^2
  mpf_div_ui(pi, pi, 4); // pi=pi/4
  mpf_div(pi, pi, t_n);  // pi=pi/tn

  mp_exp_t expoente;

  /** valor inteiro (3) + digitos + 2 digitos */
  int n_caracteres = 3 + digitos;

  // + 1 para o \0
  char *s = (char *)calloc(n_caracteres + 1, sizeof(char));

  if (s == NULL)
    return NULL;

  mpf_get_str(s, &expoente, 10, n_caracteres, pi);

  strcpy(destino, "3.");
  strncat(destino, s + 1, digitos);

  free(s);

  mpf_clear(a_0);
  mpf_clear(b_0);
  mpf_clear(t_0);
  mpf_clear(p_0);

  mpf_clear(a_n);
  mpf_clear(b_n);
  mpf_clear(t_n);
  mpf_clear(p_n);

  mpf_clear(pi);

  // se não há digitos, desconsidera o ponto
  if (digitos == 0)
    destino[1] = '\0';

  return destino;
}

/**
 * Retorna a quantidade máxima de caracteres que um int pode ter no SO
 */
unsigned long get_tamanho_caracteres_int()
{
  char str[32];
  sprintf(str, "%d", INT_MAX);
  return strlen(str);
}
