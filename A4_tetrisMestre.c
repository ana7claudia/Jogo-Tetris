#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// --------------------- Estruturas ---------------------
typedef struct {
    char nome;   // tipo da peça ('I', 'O', 'T', 'L')
    int id;      // id único
} Peca;

typedef struct {
    Peca elementos[MAX_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    Peca elementos[MAX_PILHA];
    int topo;
} Pilha;

// --------------------- Variável global para IDs ---------------------
int contadorID = 0;

// --------------------- Funções da Fila ---------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int estaCheiaFila(Fila f) {
    return f.qtd == MAX_FILA;
}

int estaVaziaFila(Fila f) {
    return f.qtd == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (!estaCheiaFila(*f)) {
        f->tras = (f->tras + 1) % MAX_FILA;
        f->elementos[f->tras] = p;
        f->qtd++;
    }
}

int desenfileirar(Fila *f, Peca *p) {
    if (!estaVaziaFila(*f)) {
        *p = f->elementos[f->frente];
        f->frente = (f->frente + 1) % MAX_FILA;
        f->qtd--;
        return 1;
    }
    return 0;
}

// --------------------- Funções da Pilha ---------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int estaCheiaPilha(Pilha p) {
    return p.topo == MAX_PILHA - 1;
}

int estaVaziaPilha(Pilha p) {
    return p.topo == -1;
}

int empilhar(Pilha *p, Peca x) {
    if (!estaCheiaPilha(*p)) {
        p->topo++;
        p->elementos[p->topo] = x;
        return 1;
    }
    return 0;
}

int desempilhar(Pilha *p, Peca *x) {
    if (!estaVaziaPilha(*p)) {
        *x = p->elementos[p->topo];
        p->topo--;
        return 1;
    }
    return 0;
}

// --------------------- Geração de peças ---------------------
Peca gerarPeca() {
    Peca nova;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// --------------------- Operações do jogo ---------------------
void jogarPeca(Fila *fila) {
    Peca removida;
    if (desenfileirar(fila, &removida)) {
        printf("Ação: Peça [%c %d] jogada!\n", removida.nome, removida.id);
        enfileirar(fila, gerarPeca()); // repõe peça
    } else {
        printf("Fila vazia!\n");
    }
}

void reservarPeca(Fila *fila, Pilha *pilha) {
    if (estaCheiaPilha(*pilha)) {
        printf("Pilha cheia! Não é possível reservar.\n");
        return;
    }
    Peca p;
    if (desenfileirar(fila, &p)) {
        empilhar(pilha, p);
        printf("Ação: Peça [%c %d] reservada na pilha!\n", p.nome, p.id);
        enfileirar(fila, gerarPeca()); // repõe peça
    }
}

void usarReserva(Pilha *pilha) {
    Peca p;
    if (desempilhar(pilha, &p)) {
        printf("Ação: Peça [%c %d] usada da pilha!\n", p.nome, p.id);
    } else {
        printf("Pilha vazia!\n");
    }
}

void trocarTopo(Fila *fila, Pilha *pilha) {
    if (estaVaziaFila(*fila) || estaVaziaPilha(*pilha)) {
        printf("Não é possível trocar. Fila ou pilha vazia.\n");
        return;
    }
    int idx = fila->frente;
    Peca temp = fila->elementos[idx];
    fila->elementos[idx] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;
    printf("Ação: Troca entre frente da fila e topo da pilha realizada!\n");
}

void trocarMultiplas(Fila *fila, Pilha *pilha) {
    if (fila->qtd < 3 || pilha->topo < 2) {
        printf("Não é possível trocar múltiplas peças. Quantidade insuficiente.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idx = (fila->frente + i) % MAX_FILA;
        Peca temp = fila->elementos[idx];
        fila->elementos[idx] = pilha->elementos[pilha->topo - i];
        pilha->elementos[pilha->topo - i] = temp;
    }
    printf("Ação: troca realizada entre os 3 primeiros da fila e as 3 peças da pilha.\n");
}

// --------------------- Visualização ---------------------
void mostrarEstado(Fila fila, Pilha pilha) {
    printf("\n================ ESTADO ATUAL ================\n");
    printf("Fila de peças:\t");
    for (int i = 0; i < fila.qtd; i++) {
        int idx = (fila.frente + i) % MAX_FILA;
        printf("[%c %d] ", fila.elementos[idx].nome, fila.elementos[idx].id);
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> Base):\t");
    for (int i = pilha.topo; i >= 0; i--) {
        printf("[%c %d] ", pilha.elementos[i].nome, pilha.elementos[i].id);
    }
    printf("\n==============================================\n");
}

// --------------------- Programa principal ---------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    do {
        mostrarEstado(fila, pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarReserva(&pilha); break;
            case 4: trocarTopo(&fila, &pilha); break;
            case 5: trocarMultiplas(&fila, &pilha); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
