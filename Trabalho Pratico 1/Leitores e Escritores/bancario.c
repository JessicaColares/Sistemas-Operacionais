#include "bancario.h"

// ==================== VARIÁVEIS GLOBAIS ====================
ContaBancaria conta;
sem_t mutex, recurso;
int leitores_ativos = 0;

// ==================== VARIÁVEIS DE ESTATÍSTICA ==============
int leituras_realizadas = 0;
int escritas_realizadas = 0;

// ==================== FUNÇÕES AUXILIARES ====================
void inicializar_conta() {
    conta.saldo = 1000.0;
    conta.ultima_transacao = 0;
    strcpy(conta.historico, "Conta criada com saldo inicial");
}

void inicializar_semaforos() {
    sem_init(&mutex, 0, 1);
    sem_init(&recurso, 0, 1);
}

void destruir_semaforos() {
    sem_destroy(&mutex);
    sem_destroy(&recurso);
}

void log_operacao(int id, char tipo, const char* operacao, double valor, double novo_saldo, const char* observacao) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    printf("[%ld.%09ld] Thread %c%d: %s", ts.tv_sec, ts.tv_nsec, tipo, id, operacao);
    
    if (valor > 0) {
        printf(" R$%.2f", valor);
    }
    
    printf(" → Saldo: R$%.2f", novo_saldo);
    
    if (strlen(observacao) > 0) {
        printf(" [%s]", observacao);
    }
    
    printf("\n");
}

void log_estado(int id, char tipo, const char* estado) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("[%ld.%09ld] Thread %c%d: %s\n", ts.tv_sec, ts.tv_nsec, tipo, id, estado);
}

// ==================== FUNÇÕES DAS THREADS ====================
void* leitor(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'L', "Iniciada");
    
    // Entrada da seção crítica - Protocolo do leitor
    sem_wait(&mutex);
    leitores_ativos++;
    if (leitores_ativos == 1) {
        sem_wait(&recurso); // Primeiro leitor bloqueia escritores
    }
    sem_post(&mutex);
    
    log_estado(id, 'L', "Entrou na região crítica");
    
    // LEITURA DOS DADOS (Região Crítica)
    double saldo_lido = conta.saldo;
    int transacao_lida = conta.ultima_transacao;
    char historico_lido[100];
    strcpy(historico_lido, conta.historico);
    
    // Simula tempo de processamento (leitor rápido)
    usleep(args->sleep_time * 1000);
    
    log_operacao(id, 'L', "Consultou saldo", 0.0, saldo_lido, historico_lido);
    leituras_realizadas++;
    
    // Saída da seção crítica
    sem_wait(&mutex);
    leitores_ativos--;
    if (leitores_ativos == 0) {
        sem_post(&recurso); // Último leitor libera escritores
    }
    sem_post(&mutex);
    
    log_estado(id, 'L', "Saiu da região crítica");
    log_estado(id, 'L', "Finalizada");
    
    free(args);
    return NULL;
}

void* escritor(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    double valor = args->valor;
    char operacao[20];
    
    if (valor >= 0) {
        strcpy(operacao, "Depositou");
    } else {
        strcpy(operacao, "Sacou");
        valor = -valor; // Converte para positivo para exibição
    }
    
    log_estado(id, 'E', "Iniciada");
    
    // Entrada da seção crítica - Escritor espera pelo recurso
    sem_wait(&recurso);
    log_estado(id, 'E', "Entrou na região crítica");
    
    // ESCRITA DOS DADOS (Região Crítica)
    double saldo_anterior = conta.saldo;
    
    // Simula tempo de processamento (escritor lento)
    usleep(args->sleep_time * 1000);
    
    // Atualiza a conta
    if (args->valor >= 0) {
        conta.saldo += args->valor;
        snprintf(conta.historico, 100, "Depósito de R$%.2f", args->valor);
    } else {
        conta.saldo += args->valor; // valor é negativo para saque
        snprintf(conta.historico, 100, "Saque de R$%.2f", -args->valor);
    }
    conta.ultima_transacao++;
    
    double novo_saldo = conta.saldo;
    
    log_operacao(id, 'E', operacao, valor, novo_saldo, "");
    escritas_realizadas++;
    
    // Saída da seção crítica
    sem_post(&recurso);
    
    log_estado(id, 'E', "Saiu da região crítica");
    log_estado(id, 'E', "Finalizada");
    
    free(args);
    return NULL;
}