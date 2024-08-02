typedef struct nodo
{
    int chave;
    int altura;
    int balanceamento;
    struct nodo *esq;
    struct nodo *dir;
} nodo_t;

/* aloca espaço na memória e inicializa um novo nodo
 * retornando um ponteiro para o mesmo */
nodo_t *criarNodo(int chave);

/* insere um novo nodo apartir da raiz fornecida e retorna 
 * um ponteiro para a árvore atualizada */
nodo_t *inserirNodo(nodo_t *raiz, int chave);

/* retorna a altura do nodo fornecido */
int altura(nodo_t *raiz);

/* retorna o maior valor entre dois inteiros */
int max(int a, int b);

/* retorna o nodo com a menor chave da árvore fornecida */
nodo_t *minimo(nodo_t *raiz);

/* retorna o fator de balanceamento do nodo fornecido */
int fatorBalanceamento(nodo_t *raiz);

/* realiza a rotação simples à direita na árvore fornecida */
nodo_t *rotacaoDireita(nodo_t *raiz);

/* realiza a rotação simples à esquerda na árvore fornecida */
nodo_t *rotacaoEsquerda(nodo_t *raiz);

/* remove um nodo apartir de uma chave fornecida e retorna um
 * ponteiro para a árvore atualizada */
nodo_t *removerNodo(nodo_t *raiz, int chave);

/* busca um nodo apartir de uma chave fornecida, escreve o caminho
 * percorrido até encontrá-lo na saida padrão e retorna um ponteiro
 * para o mesmo caso for encontrado, caso contrario retorna NULL */
nodo_t *buscarNodo(nodo_t *raiz, int chave);

/* balanceia a árvore fornecida de forma a nenhum nodo ter o balanceamento
 * maior que 1 e menor que -1 e retorna um ponteiro para a mesma */
nodo_t *balancear(nodo_t **raiz);

/* imprime a árvore fornecida em formatação "(R,E,D)" */
void imprimirArvore(nodo_t *raiz);

/* desaloca toda memória relacionada a árvore */
void destroirArvore(nodo_t *raiz);