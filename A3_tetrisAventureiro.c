#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição da estrutura para representar uma peça do jogo
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// --- Estrutura da Fila (Queue) ---
#define TAMANHO_FILA 5
Peca fila[TAMANHO_FILA];
int frente = -1;
int tras = -1;
int contador_pecas_fila = 0;
int id_contador = 0; // Contador para o ID único das peças

// Função para verificar se a fila está vazia
int fila_esta_vazia() {
    return contador_pecas_fila == 0;
}

// Função para verificar se a fila está cheia
int fila_esta_cheia() {
    return contador_pecas_fila == TAMANHO_FILA;
}

// Função para exibir o estado da fila
void exibir_fila() {
    printf("Fila de pecas\t");

    if (fila_esta_vazia()) {
        printf("[Fila vazia]\n");
        return;
    }

    int i = frente;
    int pecas_exibidas = 0;
    while (pecas_exibidas < contador_pecas_fila) {
        printf("[%c %d] ", fila[i].nome, fila[i].id);
        i = (i + 1) % TAMANHO_FILA;
        pecas_exibidas++;
    }
    printf("\n");
}

// Função para inserir uma nova peça no final da fila (enqueue)
void enqueue(Peca peca) {
    if (fila_esta_cheia()) {
        printf("\nFila cheia! Nao e possivel adicionar mais pecas.\n");
        return;
    }

    if (fila_esta_vazia()) {
        frente = 0;
    }

    tras = (tras + 1) % TAMANHO_FILA;
    fila[tras] = peca;
    contador_pecas_fila++;
}

// Função para remover uma peça da frente da fila (dequeue)
Peca dequeue() {
    if (fila_esta_vazia()) {
        Peca peca_vazia = {'\0', -1};
        return peca_vazia;
    }

    Peca peca_removida = fila[frente];
    frente = (frente + 1) % TAMANHO_FILA;
    contador_pecas_fila--;

    if (fila_esta_vazia()) {
        frente = -1;
        tras = -1;
    }
    return peca_removida;
}

// --- Estrutura da Pilha (Stack) ---
#define TAMANHO_PILHA 3
Peca pilha[TAMANHO_PILHA];
int topo = -1;

// Função para verificar se a pilha está vazia
int pilha_esta_vazia() {
    return topo == -1;
}

// Função para verificar se a pilha está cheia
int pilha_esta_cheia() {
    return topo == TAMANHO_PILHA - 1;
}

// Função para exibir o estado da pilha
void exibir_pilha() {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilha_esta_vazia()) {
        printf("[Pilha vazia]\n");
        return;
    }
    for (int i = topo; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }
    printf("\n");
}

// Função para inserir uma nova peça no topo da pilha (push)
void push(Peca peca) {
    if (pilha_esta_cheia()) {
        printf("\nPilha cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    pilha[++topo] = peca;
}

// Função para remover uma peça do topo da pilha (pop)
Peca pop() {
    if (pilha_esta_vazia()) {
        Peca peca_vazia = {'\0', -1};
        return peca_vazia;
    }
    return pilha[topo--];
}

// --- Funções Auxiliares e de Gerenciamento ---
// Função para gerar uma nova peça aleatoriamente
Peca gerar_peca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova_peca;
    nova_peca.nome = tipos[rand() % 4];
    nova_peca.id = id_contador++;
    return nova_peca;
}

// Função para exibir o menu de opções
void exibir_menu() {
    printf("\nOpcoes de Acao:\n");
    printf("Codigo\t Acao\n");
    printf("1\t Jogar peca\n");
    printf("2\t Reservar peca\n");
    printf("3\t Usar peca reservada\n");
    printf("0\t Sair\n");
    printf("------------------------\n");
}

int main() {
    srand(time(NULL));

    // Inicializa a fila com 5 peças
    while (!fila_esta_cheia()) {
        enqueue(gerar_peca());
    }

    int opcao;
    do {
        printf("\n--- Estado Atual ---\n");
        exibir_fila();
        exibir_pilha();
        exibir_menu();
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca peca_jogada = dequeue();
                if (peca_jogada.id != -1) {
                    printf("Peca %c %d jogada (removida da fila).\n", peca_jogada.nome, peca_jogada.id);
                    enqueue(gerar_peca());
                }
                break;
            }
            case 2: {
                Peca peca_reservada = dequeue();
                if (peca_reservada.id != -1) {
                    push(peca_reservada);
                    printf("Peca %c %d reservada (movida para a pilha).\n", peca_reservada.nome, peca_reservada.id);
                    enqueue(gerar_peca());
                }
                break;
            }
            case 3: {
                Peca peca_usada = pop();
                if (peca_usada.id != -1) {
                    printf("Peca %c %d usada (removida da pilha).\n", peca_usada.nome, peca_usada.id);
                    enqueue(gerar_peca());
                } else {
                    printf("\nPilha vazia! Nao ha pecas para usar.\n");
                }
                break;
            }
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