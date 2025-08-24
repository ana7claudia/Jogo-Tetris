#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Constantes e Definições ---
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// --- Estruturas de Dados ---

// Estrutura para representar uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a fila circular de peças
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura para a pilha de peças reservadas
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
} Pilha;

// --- Variáveis Globais ---
int proximoId = 0; // Contador global para IDs de peças únicas

// --- Funções Auxiliares ---

/**
 * @brief Gera uma nova peça com ID único e tipo aleatório.
 * @return Retorna a nova peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indiceTipo = rand() % 4; // Seleciona um tipo aleatório
    
    novaPeca.nome = tipos[indiceTipo];
    novaPeca.id = proximoId++; // Atribui e incrementa o ID
    
    return novaPeca;
}

// --- Funções da Fila Circular ---

/**
 * @brief Inicializa a fila circular.
 * @param f Ponteiro para a fila a ser inicializada.
 */
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param f Ponteiro para a fila.
 * @return 1 se a fila estiver vazia, 0 caso contrário.
 */
int isFilaVazia(FilaCircular *f) {
    return f->tamanho == 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @param f Ponteiro para a fila.
 * @return 1 se a fila estiver cheia, 0 caso contrário.
 */
int isFilaCheia(FilaCircular *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

/**
 * @brief Adiciona uma peça no final da fila.
 * @param f Ponteiro para a fila.
 * @param p A peça a ser enfileirada.
 */
void enfileirar(FilaCircular *f, Peca p) {
    if (isFilaCheia(f)) {
        printf("A fila esta cheia. Nao e possivel adicionar mais pecas.\n");
        return;
    }
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

/**
 * @brief Remove e retorna a peça da frente da fila.
 * @param f Ponteiro para a fila.
 * @return A peça removida.
 */
Peca desenfileirar(FilaCircular *f) {
    if (isFilaVazia(f)) {
        Peca pecaVazia = {' ', -1};
        return pecaVazia;
    }
    Peca pecaRemovida = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return pecaRemovida;
}

// --- Funções da Pilha ---

/**
 * @brief Inicializa a pilha.
 * @param p Ponteiro para a pilha a ser inicializada.
 */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @param p Ponteiro para a pilha.
 * @return 1 se a pilha estiver vazia, 0 caso contrário.
 */
int isPilhaVazia(Pilha *p) {
    return p->topo == -1;
}

/**
 * @brief Verifica se a pilha está cheia.
 * @param p Ponteiro para a pilha.
 * @return 1 se a pilha estiver cheia, 0 caso contrário.
 */
int isPilhaCheia(Pilha *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

/**
 * @brief Adiciona uma peça no topo da pilha.
 * @param p Ponteiro para a pilha.
 * @param peca A peça a ser empilhada.
 */
void empilhar(Pilha *p, Peca peca) {
    if (isPilhaCheia(p)) {
        printf("A pilha de reserva esta cheia. Nao e possivel adicionar mais pecas.\n");
        return;
    }
    p->itens[++p->topo] = peca;
}

/**
 * @brief Remove e retorna a peça do topo da pilha.
 * @param p Ponteiro para a pilha.
 * @return A peça removida.
 */
Peca desempilhar(Pilha *p) {
    if (isPilhaVazia(p)) {
        Peca pecaVazia = {' ', -1};
        return pecaVazia;
    }
    return p->itens[p->topo--];
}

// --- Funções de Visualização ---

/**
 * @brief Exibe o estado atual da fila de peças.
 * @param f Ponteiro para a fila.
 */
void visualizarFila(FilaCircular *f) {
    printf("Fila de pecas\t");
    if (isFilaVazia(f)) {
        printf("[Vazia]\n");
        return;
    }
    
    printf("\t");
    int i = f->frente;
    int count = 0;
    while (count < f->tamanho) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % CAPACIDADE_FILA;
        count++;
    }
    printf("\n");
}

/**
 * @brief Exibe o estado atual da pilha de reserva.
 * @param p Ponteiro para a pilha.
 */
void visualizarPilha(Pilha *p) {
    printf("Pilha de reserva\t");
    if (isPilhaVazia(p)) {
        printf("(Topo -> base): [Vazia]\n");
        return;
    }
    
    printf("(Topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// --- Função Principal ---
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    FilaCircular fila;
    Pilha pilha;
    int opcao;

    // 1. Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    printf("Inicializando o sistema com pecas iniciais...\n");
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    do {
        // 2. Exibe o estado atual
        printf("\n--- Estado atual: ---\n");
        visualizarFila(&fila);
        visualizarPilha(&pilha);
        printf("---------------------\n");
        
        // 3. Exibe o menu
        printf("\nOpcoes disponiveis:\n");
        printf("Codigo\tAcao\n");
        printf("1\tJogar peca da frente da fila\n");
        printf("2\tEnviar peca da fila para a pilha de reserva\n");
        printf("3\tUsar peca da pilha de reserva\n");
        printf("4\tTrocar peca da frente da fila com o topo da pilha\n");
        printf("5\tTrocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0\tSair\n");
        
        // 4. Lê a opção do usuário
        printf("\nOpcao escolhida: ");
        scanf("%d", &opcao);

        // 5. Executa a ação correspondente
        switch (opcao) {
            case 1: { // Jogar peça
                if (!isFilaVazia(&fila)) {
                    Peca pecaRemovida = desenfileirar(&fila);
                    enfileirar(&fila, gerarPeca()); // Mantém a fila cheia
                    printf("Acao: Peca [%c %d] jogada da fila.\n", pecaRemovida.nome, pecaRemovida.id);
                } else {
                    printf("Acao: Nao ha pecas para jogar na fila.\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (!isFilaVazia(&fila) && !isPilhaCheia(&pilha)) {
                    Peca pecaParaReserva = desenfileirar(&fila);
                    empilhar(&pilha, pecaParaReserva);
                    enfileirar(&fila, gerarPeca()); // Mantém a fila cheia
                    printf("Acao: Peca [%c %d] enviada da fila para a pilha de reserva.\n", pecaParaReserva.nome, pecaParaReserva.id);
                } else {
                    printf("Acao: Nao foi possivel reservar a peca (fila vazia ou pilha cheia).\n");
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (!isPilhaVazia(&pilha)) {
                    Peca pecaUsada = desempilhar(&pilha);
                    printf("Acao: Peca [%c %d] usada da pilha de reserva.\n", pecaUsada.nome, pecaUsada.id);
                } else {
                    printf("Acao: A pilha de reserva esta vazia.\n");
                }
                break;
            }
            case 4: { // Troca simples
                if (!isFilaVazia(&fila) && !isPilhaVazia(&pilha)) {
                    // Pega a peça da frente da fila e do topo da pilha
                    Peca pecaFrenteFila = fila.itens[fila.frente];
                    Peca pecaTopoPilha = pilha.itens[pilha.topo];

                    // Realiza a troca
                    fila.itens[fila.frente] = pecaTopoPilha;
                    pilha.itens[pilha.topo] = pecaFrenteFila;
                    printf("Acao: Troca realizada entre a peca da frente da fila e o topo da pilha.\n");
                } else {
                    printf("Acao: Nao foi possivel realizar a troca (fila ou pilha estao vazias).\n");
                }
                break;
            }
            case 5: { // Troca múltipla
                if (fila.tamanho >= 3 && pilha.topo == 2) {
                    printf("Acao: Troca realizada entre os 3 primeiros da fila e as 3 pecas da pilha.\n");
                    for (int i = 0; i < 3; i++) {
                        Peca temp = fila.itens[(fila.frente + i) % CAPACIDADE_FILA];
                        fila.itens[(fila.frente + i) % CAPACIDADE_FILA] = pilha.itens[i];
                        pilha.itens[i] = temp;
                    }
                } else {
                    printf("Acao: Nao foi possivel realizar a troca multipla.\n");
                    printf("Requisitos: Fila com 3+ pecas e Pilha com 3 pecas.\n");
                }
                break;
            }
            case 0: {
                printf("Encerrando o programa.\n");
                break;
            }
            default: {
                printf("Opcao invalida. Por favor, escolha uma opcao valida.\n");
                break;
            }
        }
    } while (opcao != 0);

    return 0;
}