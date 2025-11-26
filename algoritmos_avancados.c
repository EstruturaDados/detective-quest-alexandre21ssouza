#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------------------
// Estrutura que representa cada sala da mansão
// -------------------------------------------
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// -----------------------------------------------------
// criarSala()
// Cria dinamicamente uma sala com um nome específico.
// Retorna um ponteiro para a sala criada.
// -----------------------------------------------------
Sala* criarSala(char nome[]) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar sala.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// --------------------------------------------------------
// explorarSalas()
// Permite ao jogador navegar pela mansão.
// Ele começa no Hall e escolhe ir para esquerda (e) ou direita (d).
// A exploração termina quando chega a uma sala folha.
// --------------------------------------------------------
void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se é uma sala sem caminhos (folha)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração.\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf(" e - ir para a esquerda\n");
        if (atual->direita != NULL) printf(" d - ir para a direita\n");
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
            printf("Exploração encerrada.\n");
            break;
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

// --------------------------------------------------------
// main()
// Monta o mapa fixo da mansão e inicia a exploração.
// --------------------------------------------------------
int main() {

    // --- Criando as salas da mansão ---
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jardim = criarSala("Jardim");
    Sala* quarto = criarSala("Quarto Principal");

    // --- Montando a árvore binária (mapa da mansão) ---
    hall->esquerda = salaEstar;      // Hall -> esquerda -> Sala de Estar
    hall->direita = biblioteca;      // Hall -> direita -> Biblioteca

    salaEstar->esquerda = cozinha;   // Sala de Estar -> esquerda -> Cozinha
    salaEstar->direita = jardim;     // Sala de Estar -> direita -> Jardim

    biblioteca->esquerda = quarto;   // Biblioteca -> esquerda -> Quarto
    // biblioteca->direita continua NULL (folha de um lado)

    // --- Iniciar navegação ---
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall);

    return 0;
}
