#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "arff.h"

/* Recebe uma string e retorna um ponteiro para um arquivo
 * criado com o titulo da string fornecida */
static FILE *abre_arquivo(char *nome)
{
  FILE *arquivo = fopen(nome, "w+");
  if (!arquivo)
  {
    printf("Erro ao abrir o arquivo %s!\n", nome);
    exit(11);
  }
  return arquivo;
}

/* Cria um arquivo R_ATAQUES.txt que contém o nome dos ataques
 *  e seus números de ocorrencias */
void relatorio_ataques(FILE *arff, int quantidade, char *nome_arquivo)
{
  FILE *arquivo = abre_arquivo(nome_arquivo);
  char linha[2048], *ataque;
  entidade_t *vet = NULL;
  int cont = 0;
  rewind(arff);
  
  pula_atributos(arff, linha);
  
  while(!feof(arff))
  {
    ataque = strtok(linha, ",");  
    for(int i = 0; i < quantidade-2; i++)
      ataque = strtok(NULL, ",");
    ataque = strtok(NULL, "\n");

    if(!strstr(ataque, "Normal"))
    {
      // Caso o vetor de ataques esteja vazio
      if(!vet)
      {
        vet = (entidade_t*) malloc(sizeof(entidade_t));
        vet[0].ataque = strdup(ataque);
        vet[0].tamanho = 1;
        cont++;
      }
      else
      {
        int i = 0;
        // Procura o ataque no vetor de ataques
        for(int j = 0; j < cont; j++)
        {
          if(strstr(vet[j].ataque, ataque) || strstr(ataque, vet[j].ataque))
          {
            vet[j].tamanho++;
            i++;
            break;
          }
        }
        // Caso o ataque não esteja no vetor de ataques
        if(!i)
        {
          vet = (entidade_t*) realloc(vet, sizeof(entidade_t) * (cont+1));
          vet[cont].ataque = strdup(ataque);
          vet[cont].tamanho = 1;
          cont++;
        }
      }
    }
    le_linha(arff, linha, 2048);
  }
  for(int i = 0; i < cont; i++)
  {
    fprintf(arquivo, "%s;%d\n", vet[i].ataque, vet[i].tamanho);
    free(vet[i].ataque);
  }
  free(vet);

  fclose(arquivo);
}

/* Cria um arquivo R_ENTIDADES.txt que contém o ip das entidades e sua classificação,
 *  podendo ser benignas, potencialmente maliciosas e maliciosas */
void relatorio_entidade(FILE *arff, int quantidade, char *nome_arquivo)
{
  FILE *arquivo = abre_arquivo(nome_arquivo);
  char linha[2048], *ip, *ataque;
  entidade_t *vet = NULL;
  int cont = 0;
  
  rewind(arff);
  pula_atributos(arff, linha);
  
  while(!feof(arff))
  {
    ip = strtok(linha, ",");  
    for(int i = 0; i < quantidade-2; i++)
      ataque = strtok(NULL, ",");
    ataque = strtok(NULL, "\n");

    if(!vet)
    {
      vet = (entidade_t*) malloc(sizeof(entidade_t));
      vet[0].ip = strdup(ip);
      vet[0].tamanho = 0;
      if(!strstr(ataque, "Normal"))
        vet[0].tamanho = 1;
      cont++;
    }
    else
    {
      int i = 0;
      // Procura o ataque no vetor de ataques
      for(int j = 0; j < cont; j++)
      {
        if(strstr(vet[j].ip, ip) && strstr(ip, vet[j].ip))
        {
          if(!strstr(ataque, "Normal"))
            vet[j].tamanho++;
          i++;
          break;
        }
      }
      // Caso o ataque não esteja no vetor de ataques
      if(!i)
      {
        vet = (entidade_t*) realloc(vet, sizeof(entidade_t) * (cont+1));
        vet[cont].ip = strdup(ip);
        vet[cont].tamanho = 0;
        if(!strstr(ataque, "Normal"))
          vet[cont].tamanho = 1;
        cont++;
      }
    }
  le_linha(arff, linha, 2048);
  }
  for(int i = 0; i < cont; i++)
  {
    if(vet[i].tamanho == 0)
      fprintf(arquivo, "%s;benigno\n", vet[i].ip);
    else if(vet[i].tamanho <= 5)
      fprintf(arquivo, "%s;potencialmente_malicioso\n", vet[i].ip);
    else
      fprintf(arquivo, "%s;malicioso\n", vet[i].ip);
    free(vet[i].ip);
  }
  free(vet);

  fclose(arquivo);
}
  

/* Cria um arquivo R_TAMANHO.txt que contém a média da média do tamanho dos pacotes
 *  por cada tipo de ataque */
void relatorio_tamanho(FILE *arff, int quantidade, char *nome_arquivo)
{
  FILE *arquivo = abre_arquivo(nome_arquivo);
  char linha[2048], *ataque;
  int tamanho, cont = 0;
  entidade_t *vet = NULL;
  
  rewind(arff);
  pula_atributos(arff, linha);
  while(!feof(arff))
  {
    ataque = strtok(linha, ",");  
    for(int i = 0; i < quantidade-2; i++)
    {
      ataque = strtok(NULL, ",");
      if(i == 19)
        tamanho = atoi(ataque);
    }
    ataque = strtok(NULL, "\n");

    if(!strstr(ataque, "Normal"))
    {
      // Caso o vetor de ataques esteja vazio
      if(!vet)
      {
        vet = (entidade_t*) malloc(sizeof(entidade_t));
        vet[0].ataque = strdup(ataque);
        vet[0].tamanho = tamanho;
        vet[0].quantidade = 1;
        cont++;
      }
      else
      {
        int i = 0;
        // Procura o ataque no vetor de ataques
        for(int j = 0; j < cont; j++)
        {
          if(strstr(vet[j].ataque, ataque) || strstr(ataque, vet[j].ataque))
          {
            vet[j].tamanho += tamanho;
            vet[j].quantidade++;
            i++;
            break;
          }
        }
        // Caso o ataque não esteja no vetor de ataques
        if(!i)
        {
          vet = (entidade_t*) realloc(vet, sizeof(entidade_t) * (cont+1));
          vet[cont].ataque = strdup(ataque);
          vet[cont].tamanho = tamanho;
          vet[cont].quantidade = 1;
          cont++;
        }
      }
    }
    le_linha(arff, linha, 2048);
  }
  for(int i = 0; i < cont; i++)
  {
    fprintf(arquivo, "%s;%d\n", vet[i].ataque, vet[i].tamanho/vet[i].quantidade);
    free(vet[i].ataque);
  }
  free(vet);

  fclose(arquivo);
}

/* Cria um arquivo BLACKLIST.bl que contém os ips maliciosos */
void relatorio_firewall(FILE *arff, int quantidade, char *nome_arquivo)
{
    FILE *arquivo = abre_arquivo(nome_arquivo);
    relatorio_entidade(arff, quantidade, "R_ENTIDADES_TEMP.txt");
    FILE *arquivo_temp = fopen("R_ENTIDADES_TEMP.txt", "r");
    if(!arquivo_temp)
    {
      printf("Erro ao abrir o arquivo R_ENTIDADES_TEMP.txt!\n");
      exit(11);
    }
    char linha[2048], *ip, *status;
    
    le_linha(arquivo_temp, linha, 2048);
    while(!feof(arquivo_temp))
    {
      ip = strtok(linha, ";");
      status = strtok(NULL, "\n");
      if(strstr(status, "malicioso") && strstr("malicioso", status))
        fprintf(arquivo, "%s\n", ip);
        
      le_linha(arquivo_temp, linha, 2048);
    }

    fclose(arquivo);
    fclose(arquivo_temp);
    remove("R_ENTIDADES_TEMP.txt");
}
