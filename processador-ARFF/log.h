#ifndef __LOG__
#define __LOG__

#ifndef __ATRIBUTE__
#define __ATRIBUTE__

typedef struct {
  char *rotulo;
  char *tipo;
  char **categorias;
} atributo;

#endif

typedef struct {
  char* ip;
  char *ataque;
  int tamanho;
  int quantidade;
} entidade_t;

/* Cria um arquivo R_ATAQUES.txt que contém o nome dos ataques
 *  e seus números de ocorrencias */
void relatorio_ataques(FILE *arff, int quantidade, char *nome_arquivo);

/* Cria um arquivo R_ENTIDADES.txt que contém o ip das entidades e sua classificação,
 *  podendo ser benignas, potencialmente maliciosas e maliciosas */
void relatorio_entidade(FILE *arff, int quantidade, char *nome_arquivo);

/* Cria um arquivo R_TAMANHO.txt que contém a média da média do tamanho dos pacotes
 *  por cada tipo de ataque */
void relatorio_tamanho(FILE *arff, int quantidade, char *nome_arquivo);

/* Cria um arquivo BLACKLIST.bl que contém os ips maliciosos */
void relatorio_firewall(FILE *arff, int quantidade, char *nome_arquivo);

#endif