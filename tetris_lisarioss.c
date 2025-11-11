#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ---------- Estruturas ----------
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// ---------- Funções auxiliares ----------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaCheia(Fila *f) {
    return f->tamanho == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------- Operações da Fila ----------
void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->pecas[f->tras] = p;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return p;
}

// ---------- Operações da Pilha ----------
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = nova;
    printf("\n📦 Peça [%c %d] foi reservada.\n", nova.nome, nova.id);
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("\n⚠️  Pilha vazia! Não há peça reservada.\n");
        return removida;
    }
    removida = p->pecas[p->topo];
    p->topo--;
    printf("\n🎮 Peça reservada [%c %d] usada.\n", removida.nome, removida.id);
    return removida;
}

// ---------- Exibição ----------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=============================================\n");
    printf("🧩  Fila de peças: ");
    if (filaVazia(f)) printf("[vazia]");
    else {
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->frente + i) % MAX_FILA;
            printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        }
    }

    printf("\n📦  Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) printf("[vazia]");
    else {
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n=============================================\n");
}

// ---------- Trocas ----------
void trocarPeça(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  Não é possível trocar — uma das estruturas está vazia.\n");
        return;
    }

    int idxFrente = f->frente;
    Peca temp = f->pecas[idxFrente];
    f->pecas[idxFrente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;

    printf("\n🔄 Troca realizada entre a peça da frente da fila e o topo da pilha.\n");
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("\n⚠️  Troca múltipla não possível (menos de 3 peças em uma das estruturas).\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idx = (f->frente + i) % MAX_FILA;
        Peca temp = f->pecas[idx];
        f->pecas[idx] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }

    printf("\n🔁 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha.\n");
}

// ---------- Função Principal ----------
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int idGlobal = 0;

    // Inicializa fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++)
        enfileirar(&fila, gerarPeca(idGlobal++));

    int opcao;
    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("\n🎯 Peça [%c %d] jogada.\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(idGlobal++)); // Mantém fila cheia
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    enfileirar(&fila, gerarPeca(idGlobal++));
                } else {
                    printf("\n⚠️  Não foi possível reservar a peça.\n");
                }
                break;
            }
            case 3: {
                desempilhar(&pilha);
                break;
            }
            case 4:
                trocarPeça(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\n👋 Encerrando o jogo. Até a próxima!\n");
                break;
            default:
                printf("\n❌ Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}