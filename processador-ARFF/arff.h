#ifndef __ARFF__
#define __ARFF__

#ifndef __ATRIBUTE__
#define __ATRIBUTE__

typedef struct {
  char *rotulo;
  char *tipo;
  char **categorias;
} atributo;

#endif

/* Exibe todos os rótulos, tipos e categorias de um vetor de atributos de forma ordenada */
void exibe_atributos(atributo *infos, int quantidade);

/* Devolve quantos atributos bem formadas existem em uma arquivo, se houver
 * algum atributo mal formado, encerra o programa */
int conta_atributos(FILE *arff);

/* Pula a sessão de atributos de um arquivo */ 
void pula_atributos(FILE *arff, char *linha);

/* Lê uma nova linha até ela ser diferente de vazia */
void le_linha(FILE *arff, char *linha, int tam);

/* Transforma uma string de categorias em um vetor e o atribuí ao elemento passado por parametro */
void processa_categorias(atributo *elemento, char *categorias);

/* Devolve um vetor de atributos com os atributos presentes no arquivo passado */
atributo* processa_atributos(FILE *arff, int quantidade);

/* Verifica se a sessão de dados do arquivo passado condiz com o vetor de atributos,
 * se algum dado não estiver padronizado, encerra o programa */
void valida_arff(FILE *arff, atributo *atributos, int quantidade);

/* Desaloca todos os rótulos, tipos e categorias de um vetor de atributos */
void desaloca_atributos(atributo *infos, int quantidade);

#endif