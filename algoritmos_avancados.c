#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------
// Estrutura da sala (nó da árvore binária do mapa)
// Agora inclui uma pista opcional
// ---------------------------------------------------
typedef struct Sala {
    char nome[50];
    char pista[100];  // NOVO: pista associada ao cômodo
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// ---------------------------------------------------
// Estrutura da árvore BST de pistas coletadas
// ---------------------------------------------------
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// ---------------------------------------------------
// criarSala()
// Cria dinamicamente uma sala com nome e pista
// ---------------------------------------------------
Sala* criarSala(char nome[], char pista[]) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar sala.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// ---------------------------------------------------
// inserirPista()
// Insere uma pista na BST seguindo as regras de ordenação
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
// exibirPistas()
// Exibe as pistas coletadas em ordem alfabética (in-order)
// ---------------------------------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// ---------------------------------------------------
// explorarSalasComPistas()
// Permite ao jogador navegar e coleta pistas automaticamente
// ---------------------------------------------------
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta a pista automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista neste local.\n");
        }

        // Opções de navegação
        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL) printf(" e - seguir para a esquerda\n");
        if (atual->direita != NULL) printf(" d - seguir para a direita\n");
        printf(" s - sair da exploração\n");
        printf("Digite sua opção: ");

        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nExploração encerrada pelo detetive.\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

// ---------------------------------------------------
// main()
// Monta o mapa e inicia a exploração com coleta de pistas
// ---------------------------------------------------
int main() {

    // Criando as salas com suas pistas
    Sala* hall        = criarSala("Hall de Entrada",         "Pegadas recentes no tapete.");
    Sala* salaEstar   = criarSala("Sala de Estar",           "Marcas de luva deixadas no sofá.");
    Sala* biblioteca  = criarSala("Biblioteca",              "Um livro arrancado da estante.");
    Sala* cozinha     = criarSala("Cozinha",                 "Copo quebrado no chão.");
    Sala* jardim      = criarSala("Jardim",                  "Galho quebrado próximo ao portão.");
    Sala* quarto      = criarSala("Quarto Principal",        "Janela aberta com vento forte.");

    // Montando a árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita  = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita  = jardim;

    biblioteca->esquerda = quarto;

    // Árvore BST de pistas coletadas
    PistaNode* arvorePistas = NULL;

    printf("=== Detective Quest: Nível Aventureiro ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibir pistas ao final
    printf("\n=== PISTAS COLETADAS ===\n");

    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi encontrada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    return 0;
}
