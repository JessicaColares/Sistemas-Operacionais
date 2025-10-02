#ifndef LOJA_H
#define LOJA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 5
#define NUM_ETAPAS 5
#define TAM_ETAPA 40  // AUMENTEI DE 20 PARA 40

// ==================== ESTRUTURAS DE DADOS ====================
typedef struct {
    int id_pedido;
    char etapa[TAM_ETAPA];  // AUMENTEI O TAMANHO
} Pedido;

typedef struct {
    Pedido pedidos[BUFFER_SIZE];
    int inicio;
    int fim;
    int count;
} Buffer;

typedef struct {
    int id;
    char tipo;  // 'P' para produtor, 'C' para consumidor
    int sleep_time;
} ThreadArgs;

// ==================== VARIÁVEIS GLOBAIS ====================
extern Buffer buffer;
extern int pedido_id_counter;
extern const char* etapas_entrega[NUM_ETAPAS];

// ==================== SEMÁFOROS ====================
extern sem_t vazio;
extern sem_t cheio;
extern sem_t mutex;

// ==================== FUNÇÕES AUXILIARES ====================
void inicializar_buffer();
void inicializar_semaforos();
void destruir_semaforos();
void mostrar_buffer();
void log_operacao(int id, char tipo, const char* operacao, int pedido_id, const char* etapa);
void log_estado(int id, char tipo, const char* estado);
const char* proxima_etapa(const char* etapa_atual);

#endif