#include "bancario.h"
#include "v1.h"
#include "v2.h"
#include "v3.h"
#include <sys/time.h>

// Função para obter timestamp em milissegundos
double get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    int num_leitores, num_escritores;
    
    if (argc == 3) {
        num_leitores = atoi(argv[1]);
        num_escritores = atoi(argv[2]);
    } else {
        num_leitores = 3;
        num_escritores = 2;
        printf("Uso: %s <num_leitores> <num_escritores>\n", argv[0]);
        printf("Usando valores padrão: %d leitores, %d escritores\n\n", num_leitores, num_escritores);
    }
    
    printf("SISTEMA BANCÁRIO - LEITORES/ESCRITORES\n");
    printf("=======================================\n");
    
    double start_time, end_time, execution_time;
    
    // ========== VERSÃO 1 ==========
    inicializar_conta();
    start_time = get_timestamp();
    versao_sem_prioridade(num_leitores, num_escritores);
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 1:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Leituras realizadas: %d\n", leituras_realizadas);
    printf("   Escritas realizadas: %d\n", escritas_realizadas);
    printf("   Saldo final: R$%.2f\n", conta.saldo);
    
    // ========== VERSÃO 2 ==========
    inicializar_conta();
    start_time = get_timestamp();
    versao_escritores_prioridade(num_leitores, num_escritores);
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 2:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Leituras realizadas: %d\n", leituras_realizadas_v2);
    printf("   Escritas realizadas: %d\n", escritas_realizadas_v2);
    printf("   Saldo final: R$%.2f\n", conta.saldo);
    
    // ========== VERSÃO 3 ==========
    inicializar_conta();
    start_time = get_timestamp();
    versao_sem_controle(num_leitores, num_escritores);
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 3:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Leituras realizadas: %d\n", leituras_realizadas_v3);
    printf("   Escritas realizadas: %d\n", escritas_realizadas_v3);
    printf("   Saldo final: R$%.2f\n", conta.saldo);
    
    return 0;
}