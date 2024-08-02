#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "arff.h"

/* Exibe todos os rótulos, tipos e categorias de um vetor de atributos de forma ordenada */
void exibe_atributos(atributo *infos, int quantidade)
{
  if (infos == 0)
  {
    printf("O arquivo ARFF fornecido � inv�lido!\n");
    exit(0);
  }

  printf("==== ATRIBUTOS DO ARQUIVO ====\n");
  for (int i = 0; i < quantidade; i++)
  {
    printf("-> Atributo #%d\n", i + 1);
    printf("Rótulo: %s\n", infos[i].rotulo);
    printf("Tipo: %s\n", infos[i].tipo);
    if (infos[i].categorias)
    {
      printf("Categorias: ");
      for(int j = 0; infos[i].categorias[j]; j++)
        printf("%s ", infos[i].categorias[j]);
      printf("\n");
    }
    if (i < quantidade - 1)
      printf("------------------------------\n");
  }
  printf("===============================\n");
}

/* Devolve a linha atual de um arquivo passado por parametro */
static int calcula_linha_atual(FILE *arff)
{
    long int posicao = ftell(arff);
    int n_linha = 0;
    char linha[1025];

    rewind(arff);
    while (ftell(arff) != posicao)
    {
        fgets(linha, 1024, arff);
        n_linha++;
    }

    return n_linha;
}

/* Lê uma nova linha até ela ser diferente de vazia */
void le_linha(FILE *arff, char *linha, int tam)
{
  fgets(linha, tam, arff);
  while(!feof(arff) && linha[0] == '\n')
    fgets(linha, tam, arff);
}

/* Pula a sessão de atributos de um arquivo */ 
void pula_atributos(FILE *arff, char *linha)
{
  le_linha(arff, linha, 2048);
  while(!strstr(linha, "@data"))
    le_linha(arff, linha, 2048);
  le_linha(arff, linha, 2048);
}

/* Retorna 1 se o atributo da linha fornecida for bem formado,
 * caso contrário retorna 0 */
int valida_atributo(char *linha)
{
  if(strcmp(strtok(linha, " "), "@attribute"))
    return 0;

  int cont = 0;
  
  while(strtok(NULL, " "))
      cont++;
    
  if(cont != 2)
    return 0;

  return 1;
} 

/* Devolve quantos atributos bem formadas existem em uma arquivo, se houver
 * algum atributo mal formado, encerra o programa */
int conta_atributos(FILE *arff)
{
  int cont = 0;
  char linha[1025];
  
  le_linha(arff, linha, 1024);
  while (!strstr(linha, "@data"))
  {
    if(!valida_atributo(linha))
    {
      printf("Atributo mal formado na linha %d!\n", calcula_linha_atual(arff));
      exit(5);
    }
    cont++;

    le_linha(arff, linha, 1024);
  }
  
  if(!strstr(linha, "@data"))
  {
    printf("@data não encontrado!\n");
    exit(6);
  }
  rewind(arff);

  return cont;
}

/* Transforma uma string de categorias em um vetor e o atribuí ao elemento passado por parametro */
void processa_categorias(atributo *elemento, char *categorias)
{
  char *token = strdup(categorias);
  char *aux_token = token;
  int cont = 1;

  strtok(token, ",");
  while(strtok(NULL, ","))
    cont++;

  elemento->categorias = (char**) malloc(sizeof(char*) * (cont+1));  
  for(int i = 0; i < cont; i++)
    elemento->categorias[i] = (char*) malloc(sizeof(char) * 1024);

  char *aux = strtok(categorias, "{");
  aux = strtok(aux, ",");
  strcpy(elemento->categorias[0], aux);
  for(int i = 1; i < cont-1; i++)
  {
    aux = strtok(NULL, ",");
    strcpy(elemento->categorias[i], aux);
  }
  aux = strtok(NULL, "}");
  strcpy(elemento->categorias[cont-1], aux);
  // Sentinela para o vetor de categorias
  elemento->categorias[cont] = NULL;

  free(aux_token);
}

/* Devolve um vetor de atributos com os atributos presentes no arquivo passado */
atributo *processa_atributos(FILE *arff, int quantidade)
{
  atributo *infos = (atributo*) malloc(sizeof(atributo) * (quantidade));
  char linha[1025];
  int i = 0;

  le_linha(arff, linha, 1024);
  while (!strstr(linha, "@data"))
  {
    strtok(linha, " ");
    infos[i].rotulo = strdup(strtok(NULL, " "));
    infos[i].tipo = strdup(strtok(NULL, " "));
    // Se o tipo for categórico, processa as categorias
    if(strstr(infos[i].tipo, "{"))
    {
      processa_categorias(&infos[i], infos[i].tipo);
      free(infos[i].tipo);
      infos[i].tipo = strdup("categoric");
    }
    else
      infos[i].categorias = NULL;

    i++;
    le_linha(arff, linha, 1024);
  }
  rewind(arff);

  return infos;
}

/* Verifica se a sessão de dados do arquivo passado condiz com o vetor de atributos,
 * se algum dado não estiver padronizado, encerra o programa */
void valida_arff(FILE *arff, atributo *atributos, int quantidade)
{
  char linha[2049], *token, *aux_token;
  int cont, categoric; 

  pula_atributos(arff, linha);
  while(!feof(arff))
  {
    token = strdup(linha);
    aux_token = token;
    
    cont = 1;
    strtok(token, ",");
    while (strtok(NULL, ","))
      cont++;
    if(cont != quantidade)
    {
      printf("Erro na linha %d: quantidade de dados inválida!\n", calcula_linha_atual(arff));
      exit(7);
    }

    token = strtok(linha, ",");
    for(int i = 0; i < quantidade; i++)
    {
      categoric = 0;
      if(strstr(atributos[i].tipo, "numeric"))
      {
        for(int j = 0; j < (int)strlen(token); j++)
        {
          if(!isdigit(token[j]) && token[j] != '.' && token[j] != '-' && token[j] != '\n')
          {
            printf("Erro na linha %d: dado %s não é numérico!\n", calcula_linha_atual(arff), token);
            exit(8);
          }
        }
      }
      else
      if(!strcmp(atributos[i].tipo, "categoric"))
      {
        for(int j = 0; atributos[i].categorias[j]; j++)
        {
          if(strstr(token, atributos[i].categorias[j]) && strstr(atributos[i].categorias[j], token))
          {
            categoric = 1;
            break;
          } 
        }
        if(!categoric)
        {
          printf("Erro na linha %d: dado %s não é categórico!\n", calcula_linha_atual(arff), token);
          exit(9);
        }
      }
      if(i == quantidade-2)
        token = strtok(NULL, "\n");
      else
        token = strtok(NULL, ",");
    }
    free(aux_token);
    le_linha(arff, linha, 2048);
  }
  printf("Arquivo ARFF válido!\n");
}

/* Desaloca todos os rótulos, tipos e categorias de um vetor de atributos */
void desaloca_atributos(atributo *infos, int quantidade)
{
  for(int i = 0; i < quantidade; i++)
  {
    free(infos[i].rotulo);
    free(infos[i].tipo);
    if(infos[i].categorias)
    {
      for(int j = 0; infos[i].categorias[j]; j++)
        free(infos[i].categorias[j]);
      free(infos[i].categorias);
    }
  }
  free(infos);
}