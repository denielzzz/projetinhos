#include <stdio.h>
#include "avl-tree.h"

int main()
{
    nodo_t *raiz = NULL;
    char opcao;
    int chave;

    opcao = getchar();
    while(opcao != 'f')
    {
        switch(opcao)
        {
            case 'i':
                scanf("%d", &chave);
                printf("%c %d\n", opcao, chave);
                raiz = inserirNodo(raiz, chave);
                imprimirArvore(raiz);
                printf("\n");
                break;
            case 'r':
                scanf("%d", &chave);
                printf("%c %d\n", opcao, chave);
                raiz = removerNodo(raiz, chave);
                imprimirArvore(raiz);
                printf("\n");
                break;
            case 'b':
                scanf("%d", &chave);
                printf("%c %d\n", opcao, chave);
                buscarNodo(raiz, chave);
                printf("\n");
                break;
            default:
                destroirArvore(raiz);
                return 0;
        }
        getchar();
        opcao = getchar();
    }

    destroirArvore(raiz);

    return 0;
}