#include "bancario.h"
#include "v1.h"

void versao_sem_prioridade(int num_leitores, int num_escritores) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("VERSÃO 1 - LEITORES/ESCRITORES SEM PRIORIDADE\n");
    printf("Saldo inicial: R$%.2f\n", conta.saldo);
    printf("Leitores: %d, Escritores: %d\n", num_leitores, num_escritores);
    printf("═══════════════════════════════════════════════════\n\n");
    
    // Reinicializa contadores
    leituras_realizadas = 0;
    escritas_realizadas = 0;
    
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
        
        pthread_create(&threads[thread_index++], NULL, leitor, args);
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
        
        pthread_create(&threads[thread_index++], NULL, escritor, args);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_leitores + num_escritores; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n═══════════════════════════════════════════════════\n");
    printf("FIM DA EXECUÇÃO - Saldo final: R$%.2f\n", conta.saldo);
    printf("═══════════════════════════════════════════════════\n");
    
    destruir_semaforos();
}