#include "bancario.h"
#include "v2.h"

// Variáveis específicas para a versão 2 (escritores com prioridade)
sem_t escritor_esperando;  // Semáforo para controle de prioridade
sem_t entrada;             // Semáforo adicional para controle de entrada
int escritores_esperando = 0;

// Variáveis para estatísticas
int leituras_realizadas_v2 = 0;
int escritas_realizadas_v2 = 0;

void* leitor_prioridade(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'L', "Iniciada");
    
    // Verifica se há escritores esperando (prioridade para escritores)
    sem_wait(&entrada);
    sem_post(&entrada);
    
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
    leituras_realizadas_v2++;
    
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

void* escritor_prioridade(void* arg) {
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
    
    // Entrada da seção crítica - Protocolo do escritor com prioridade
    sem_wait(&escritor_esperando);
    escritores_esperando++;
    if (escritores_esperando == 1) {
        sem_wait(&entrada); // Primeiro escritor bloqueia leitores
    }
    sem_post(&escritor_esperando);
    
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
    escritas_realizadas_v2++;
    
    // Saída da seção crítica
    sem_post(&recurso);
    
    sem_wait(&escritor_esperando);
    escritores_esperando--;
    if (escritores_esperando == 0) {
        sem_post(&entrada); // Último escritor libera leitores
    }
    sem_post(&escritor_esperando);
    
    log_estado(id, 'E', "Saiu da região crítica");
    log_estado(id, 'E', "Finalizada");
    
    free(args);
    return NULL;
}

void versao_escritores_prioridade(int num_leitores, int num_escritores) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("VERSÃO 2 - ESCRITORES COM PRIORIDADE\n");
    printf("Saldo inicial: R$%.2f\n", conta.saldo);
    printf("Leitores: %d, Escritores: %d\n", num_leitores, num_escritores);
    printf("═══════════════════════════════════════════════════\n\n");
    
    // Reinicializa contadores
    leituras_realizadas_v2 = 0;
    escritas_realizadas_v2 = 0;
    escritores_esperando = 0;
    
    // Inicializa semáforos específicos da versão 2
    sem_init(&escritor_esperando, 0, 1);
    sem_init(&entrada, 0, 1);
    inicializar_semaforos();
    
    pthread_t threads[num_leitores + num_escritores];
    int thread_index = 0;
    
    // Cria threads leitoras
    for (int i = 0; i < num_leitores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'L';
        args->valor = 0.0;
        args->sleep_time = 100 + (rand() % 200); // 100-300ms (leitores rápidos)
        
        pthread_create(&threads[thread_index++], NULL, leitor_prioridade, args);
    }
    
    // Cria threads escritoras
    for (int i = 0; i < num_escritores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'E';
        
        // Alterna entre depósitos e saques
        if (i % 2 == 0) {
            args->valor = 100.0 + (rand() % 400); // Depósito: R$100-500
        } else {
            args->valor = -(50.0 + (rand() % 150)); // Saque: R$50-200 (negativo)
        }
        
        args->sleep_time = 300 + (rand() % 400); // 300-700ms (escritores lentos)
        
        pthread_create(&threads[thread_index++], NULL, escritor_prioridade, args);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_leitores + num_escritores; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n═══════════════════════════════════════════════════\n");
    printf("FIM DA EXECUÇÃO - Saldo final: R$%.2f\n", conta.saldo);
    printf("═══════════════════════════════════════════════════\n");
    
    sem_destroy(&escritor_esperando);
    sem_destroy(&entrada);
    destruir_semaforos();
}