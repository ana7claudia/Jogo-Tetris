#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura para representar uma peça do jogo
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Definição do tamanho máximo da fila de peças
#define TAMANHO_FILA 5

// Declaração de variáveis globais para a fila e seus controles
Peca fila[TAMANHO_FILA];
int frente = -1;  // Índice da frente da fila
int tras = -1;    // Índice do final da fila
int contador_pecas = 0; // Contador de peças na fila
int id_contador = 0;    // Contador para o ID único das peças

// Função para verificar se a fila está vazia
int esta_vazia() {
    return contador_pecas == 0;
}

// Função para verificar se a fila está cheia
int esta_cheia() {
    return contador_pecas == TAMANHO_FILA;
}

// Função para exibir o estado atual da fila
void exibir_fila() {
    printf("\n--- Estado da Fila ---");
    printf("\nFila de pecas:\n");

    if (esta_vazia()) {
        printf("[Fila vazia]\n");
        return;
    }

    int i = frente;
    int pecas_exibidas = 0;
    while (pecas_exibidas < contador_pecas) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        i = (i + 1) % TAMANHO_FILA; // Movimenta para o próximo índice de forma circular
        pecas_exibidas++;
    }
    printf("\n");
}

// Função para gerar uma nova peça aleatoriamente
Peca gerar_peca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova_peca;
    nova_peca.nome = tipos[rand() % 4]; // Seleciona um tipo aleatório
    nova_peca.id = id_contador++;       // Atribui e incrementa o ID
    return nova_peca;
}

// Função para inserir uma nova peça no final da fila (enqueue)
void enqueue(Peca peca) {
    if (esta_cheia()) {
        printf("\nFila cheia! Nao e possivel adicionar mais pecas.\n");
        return;
    }

    if (esta_vazia()) {
        frente = 0;
    }

    tras = (tras + 1) % TAMANHO_FILA;
    fila[tras] = peca;
    contador_pecas++;
    printf("\nPeca %c %d adicionada a fila.\n", peca.nome, peca.id);
}

// Função para remover uma peça da frente da fila (dequeue)
void dequeue() {
    if (esta_vazia()) {
        printf("\nFila vazia! Nao ha pecas para jogar.\n");
        return;
    }

    Peca peca_removida = fila[frente];
    frente = (frente + 1) % TAMANHO_FILA;
    contador_pecas--;

    // Se a fila ficar vazia, resetar os índices
    if (contador_pecas == 0) {
        frente = -1;
        tras = -1;
    }

    printf("\nPeca %c %d jogada (removida da frente).\n", peca_removida.nome, peca_removida.id);
}

// Função para exibir o menu de opções
void exibir_menu() {
    printf("\n--- Opcoes de Acao ---\n");
    printf("Codigo\tAcao\n");
    printf("1\tJogar peca (dequeue)\n");
    printf("2\tInserir nova peca (enqueue)\n");
    printf("0\tSair\n");
    printf("------------------------\n");
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(gerar_peca());
    }

    int opcao;
    do {
        exibir_fila();
        exibir_menu();
        printf("Digite a sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue();
                break;
            case 2:
                enqueue(gerar_peca());
                break;
            case 0:
                printf("\nSaindo do programa. Tchau!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}