#include "loja.h"

// ==================== VARIÁVEIS GLOBAIS ====================
Buffer buffer;
int pedido_id_counter = 1;

// Etapas do processo de entrega
const char* etapas_entrega[NUM_ETAPAS] = {
    "em separacao",
    "despachado", 
    "em transito",
    "saiu p/ entrega",
    "entregue"
};

// ==================== SEMÁFOROS ====================
sem_t vazio, cheio, mutex;

// ==================== FUNÇÕES AUXILIARES ====================
void inicializar_buffer() {
    buffer.inicio = 0;
    buffer.fim = 0;
    buffer.count = 0;
    pedido_id_counter = 1;
}

void inicializar_semaforos() {
    sem_init(&vazio, 0, BUFFER_SIZE);
    sem_init(&cheio, 0, 0);
    sem_init(&mutex, 0, 1);
}

void destruir_semaforos() {
    sem_destroy(&vazio);
    sem_destroy(&cheio);
    sem_destroy(&mutex);
}

const char* proxima_etapa(const char* etapa_atual) {
    for (int i = 0; i < NUM_ETAPAS - 1; i++) {
        if (strcmp(etapa_atual, etapas_entrega[i]) == 0) {
            return etapas_entrega[i + 1];
        }
    }
    return etapa_atual; // Se for a última etapa, mantém
}

void mostrar_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < buffer.count) {
            int index = (buffer.inicio + i) % BUFFER_SIZE;
            // MOSTRA APENAS OS PRIMEIROS 15 CARACTERES PARA CABER NO TERMINAL
            char etapa_abreviada[16];
            strncpy(etapa_abreviada, buffer.pedidos[index].etapa, 15);
            etapa_abreviada[15] = '\0';
            printf(" #%d(%.15s)", buffer.pedidos[index].id_pedido, etapa_abreviada);
        } else {
            printf(" ---");
        }
    }
    printf(" ]\n");
}

void log_operacao(int id, char tipo, const char* operacao, int pedido_id, const char* etapa) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("[%ld.%09ld] Thread %c%d: %s pedido #%d [%s] ", 
           ts.tv_sec, ts.tv_nsec, tipo, id, operacao, pedido_id, etapa);
    mostrar_buffer();
}

void log_estado(int id, char tipo, const char* estado) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("[%ld.%09ld] Thread %c%d: %s\n", ts.tv_sec, ts.tv_nsec, tipo, id, estado);
}