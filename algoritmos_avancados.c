#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10   // tamanho da tabela hash

// ---------------------------------------------------
// Estrutura da sala (nó da árvore binária do mapa)
// ---------------------------------------------------
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// ---------------------------------------------------
// Nó da BST de pistas coletadas
// ---------------------------------------------------
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// ---------------------------------------------------
// Estrutura para tabela hash: pista -> suspeito
// ---------------------------------------------------
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

// ---------------------------------------------------
// Função auxiliar: hash (transforma string em índice)
// ---------------------------------------------------
int hash(char* str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

// ---------------------------------------------------
// inserirNaHash()
// Associa pista → suspeito na tabela hash
// ---------------------------------------------------
void inserirNaHash(char* pista, char* suspeito) {
    int index = hash(pista);

    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[index];
    tabelaHash[index] = novo;
}

// ---------------------------------------------------
// encontrarSuspeito()
// Retorna o suspeito associado a uma pista
// ---------------------------------------------------
char* encontrarSuspeito(char* pista) {
    int index = hash(pista);
    HashNode* atual = tabelaHash[index];

    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// ---------------------------------------------------
// criarSala()
// Cria dinamicamente um cômodo da mansão
// ---------------------------------------------------
Sala* criarSala(char nome[], char pista[]) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// ---------------------------------------------------
// inserirPista()
// Insere pista na BST
// ---------------------------------------------------
PistaNode* inserirPista(PistaNode* raiz, char pista[]) {
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

// ---------------------------------------------------
// exibirPistas() – in-order
// ---------------------------------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ---------------------------------------------------
// explorarSalas()
// Faz a navegação e coleta de pistas
// ---------------------------------------------------
void explorarSalas(Sala* atual, PistaNode** bstPistas) {
    char opcao;

    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        // Coletar pista da sala
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *bstPistas = inserirPista(*bstPistas, atual->pista);
        } else {
            printf("Nenhuma pista aqui.\n");
        }

        // Mostrar opções
        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL) printf(" e - seguir para a esquerda\n");
        if (atual->direita != NULL) printf(" d - seguir para a direita\n");
        printf(" s - sair da exploração\n");

        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

// ---------------------------------------------------
// contarSuspeito() – conta pistas associadas ao suspeito
// ---------------------------------------------------
int contarSuspeito(PistaNode* raiz, char* suspeito) {
    if (raiz == NULL) return 0;

    int cont = 0;
    char* s = encontrarSuspeito(raiz->pista);

    if (s != NULL && strcmp(s, suspeito) == 0)
        cont++;

    cont += contarSuspeito(raiz->esquerda, suspeito);
    cont += contarSuspeito(raiz->direita, suspeito);

    return cont;
}

// ---------------------------------------------------
// verificarSuspeitoFinal()
// Lógica de julgamento final
// ---------------------------------------------------
void verificarSuspeitoFinal(PistaNode* bst) {
    char acusacao[50];

    printf("\n=== FASE FINAL: ACUSAÇÃO ===\n");
    printf("Digite o nome do suspeito que deseja acusar:\n> ");
    scanf(" %[^\n]", acusacao);

    int qtd = contarSuspeito(bst, acusacao);

    if (qtd >= 2)
        printf("\nACUSAÇÃO CORRETA! %s realmente é o culpado.\n", acusacao);
    else
        printf("\nACUSAÇÃO INCORRETA. %s não possui pistas suficientes contra ele.\n", acusacao);
}

// ---------------------------------------------------
// main() – monta tudo
// ---------------------------------------------------
int main() {

    // ------------ Criando salas ------------
    Sala* hall       = criarSala("Hall",               "Pegadas suspeitas no tapete.");
    Sala* salaEstar  = criarSala("Sala de Estar",      "Luvas jogadas atrás do sofá.");
    Sala* biblioteca = criarSala("Biblioteca",         "Livro arrancado da prateleira.");
    Sala* cozinha    = criarSala("Cozinha",            "Copo quebrado no chão.");
    Sala* jardim     = criarSala("Jardim",             "Galho quebrado perto do muro.");
    Sala* quarto     = criarSala("Quarto Principal",   "Perfume forte no ar.");

    // ------------ Montando a árvore da mansão ------------
    hall->esquerda = salaEstar;
    hall->direita  = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = jardim;

    biblioteca->esquerda = quarto;

    // ------------ Criando tabela hash pista → suspeito ------------
    inserirNaHash("Pegadas suspeitas no tapete.",      "Carlos");
    inserirNaHash("Luvas jogadas atrás do sofá.",      "Marina");
    inserirNaHash("Livro arrancado da prateleira.",    "Carlos");
    inserirNaHash("Copo quebrado no chão.",            "João");
    inserirNaHash("Galho quebrado perto do muro.",     "Marina");
    inserirNaHash("Perfume forte no ar.",              "João");

    // ------------ BST das pistas coletadas ------------
    PistaNode* arvorePistas = NULL;

    printf("=== DETECTIVE QUEST – NÍVEL MESTRE ===\n");

    // Exploração
    explorarSalas(hall, &arvorePistas);

    // Listar pistas ao final
    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Julgamento Final
    verificarSuspeitoFinal(arvorePistas);

    return 0;
}
