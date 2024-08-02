#include <stdio.h>
#include <stdlib.h>
#include "avl-tree.h"

nodo_t *criarNodo(int chave)
{
    nodo_t *novoNodo;
    if(!(novoNodo = (nodo_t *)malloc(sizeof(nodo_t))))
    {
        printf("Erro ao alocar memoria!\n");
        return NULL;
    }   

    novoNodo->chave = chave;
    novoNodo->altura = 1;
    novoNodo->balanceamento = 0;
    novoNodo->esq = NULL;
    novoNodo->dir = NULL;

    return novoNodo;
}

nodo_t *inserirNodo(nodo_t *raiz, int chave)
{
    if(raiz == NULL)
        return criarNodo(chave);
    if(chave < raiz->chave)
        raiz->esq = inserirNodo(raiz->esq, chave);
    else 
    if(chave > raiz->chave)
        raiz->dir = inserirNodo(raiz->dir, chave);
    else
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    raiz->balanceamento = fatorBalanceamento(raiz);
    
    if(raiz->balanceamento > 1 || raiz->balanceamento < -1)
        return balancear(&raiz);

    return raiz;
}

int altura(nodo_t *raiz)
{
    if(raiz == NULL)
        return 0;
    return raiz->altura;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

nodo_t *minimo(nodo_t *raiz)
{
    nodo_t *atual = raiz;
    while(atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

int fatorBalanceamento(nodo_t *raiz)
{
    if(raiz == NULL)
        return 0;
    return altura(raiz->esq) - altura(raiz->dir);
}

nodo_t *rotacaoDireita(nodo_t *raiz)
{
    nodo_t *aux = raiz->esq;
    nodo_t *aux2 = aux->dir;

    aux->dir = raiz;
    raiz->esq = aux2;

    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = max(altura(aux->esq), altura(aux->dir)) + 1;

    raiz->balanceamento = fatorBalanceamento(raiz);
    aux->balanceamento = fatorBalanceamento(aux);

    return aux;
}

nodo_t *rotacaoEsquerda(nodo_t *raiz)
{
    nodo_t *aux = raiz->dir;
    nodo_t *aux2 = aux->esq;

    aux->esq = raiz;
    raiz->dir = aux2;

    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = max(altura(aux->esq), altura(aux->dir)) + 1;

    raiz->balanceamento = fatorBalanceamento(raiz);
    aux->balanceamento = fatorBalanceamento(aux);

    return aux;
}

nodo_t *removerNodo(nodo_t *raiz, int chave)
{
    if(raiz == NULL)
        return raiz;
    if(chave < raiz->chave)
        raiz->esq = removerNodo(raiz->esq, chave);
    else 
    if(chave > raiz->chave)
        raiz->dir = removerNodo(raiz->dir, chave);
    else
    {
        if(raiz->esq == NULL || raiz->dir == NULL)
        {
            nodo_t *aux = raiz->esq ? raiz->esq : raiz->dir;
            
            if(aux == NULL)
            {
                aux = raiz;
                raiz = NULL;
            }
            else
                *raiz = *aux;
            
            free(aux);
        }
        else
        {
            nodo_t *aux = minimo(raiz->dir);

            raiz->chave = aux->chave;
            raiz->dir = removerNodo(raiz->dir, aux->chave);
        }
    }

    if(raiz == NULL)
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    raiz->balanceamento = fatorBalanceamento(raiz);

    if(raiz->balanceamento > 1 || raiz->balanceamento < -1)
        return balancear(&raiz);

    return raiz;
}

nodo_t *buscarNodo(nodo_t *raiz, int chave)
{
    if(raiz == NULL)
        return NULL;

    if(raiz->chave == chave)
    {
        printf("%d", raiz->chave);  
        return raiz;
    }
    
    if(chave < raiz->chave)
    {
        if(raiz->esq == NULL)
            printf("%d", raiz->chave);
        else
            printf("%d,", raiz->chave);  

        return buscarNodo(raiz->esq, chave);
    }

    if(raiz->dir == NULL)
        printf("%d", raiz->chave);
    else
        printf("%d,", raiz->chave);  

    return buscarNodo(raiz->dir, chave);
}

nodo_t *balancear(nodo_t **raiz)
{
    if(*raiz == NULL)
        return *raiz;

    if((*raiz)->balanceamento > 1)
    {
        if((*raiz)->esq->balanceamento >= 0)
            *raiz = rotacaoDireita(*raiz);
        else
        {
            (*raiz)->esq = rotacaoEsquerda((*raiz)->esq);
            *raiz = rotacaoDireita(*raiz);
        }
    }
    else 
    if((*raiz)->balanceamento < -1)
    {
        if((*raiz)->dir->balanceamento <= 0)
            *raiz = rotacaoEsquerda(*raiz);
        else
        {
            (*raiz)->dir = rotacaoDireita((*raiz)->dir);
            *raiz = rotacaoEsquerda(*raiz);
        }
    }
    
    return *raiz;
}

void imprimirArvore(nodo_t *raiz) 
{
    if (raiz == NULL) {
        printf("()");
        return;
    }

    printf("(%d,", raiz->chave);
    imprimirArvore(raiz->esq);
    printf(",");
    imprimirArvore(raiz->dir);
    printf(")");
}

void destroirArvore(nodo_t *raiz)
{
    if(raiz != NULL)
    {
        destroirArvore(raiz->esq);
        destroirArvore(raiz->dir);
        free(raiz);
    }
}