#include "loja.h"
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
    
    int num_produtores, num_consumidores;
    
    if (argc == 3) {
        num_produtores = atoi(argv[1]);
        num_consumidores = atoi(argv[2]);
    } else {
        num_produtores = 2;
        num_consumidores = 1;
        printf("Uso: %s <num_func_separacao> <num_func_entrega>\n", argv[0]);
        printf("Usando valores padrão: %d funcionários separação, %d funcionários entrega\n\n", 
               num_produtores, num_consumidores);
    }
    
    printf(" SISTEMA DE LOGÍSTICA - SEPARAÇÃO E ENTREGA DE PEDIDOS \n");
    printf("==========================================================\n");
    printf("ETAPAS: em separação → despachado → em trânsito → saiu p/ entrega → entregue\n\n");
    
    double start_time, end_time, execution_time;
    
    // ========== VERSÃO 1 ==========
    printf(" EXECUTANDO VERSÃO 1: COM CONTROLE (1 ENTREGADOR)\n");
    start_time = get_timestamp();
    versao_1_consumidor(num_produtores, 1); // Sempre 1 consumidor na V1
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 1:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Pedidos separados: %d\n", pedidos_produzidos_v1);
    printf("   Pedidos entregues: %d\n", pedidos_processados_v1);
    
    // ========== VERSÃO 2 ==========
    printf("\n EXECUTANDO VERSÃO 2: COM CONTROLE (VÁRIOS ENTREGADORES)\n");
    start_time = get_timestamp();
    versao_varios_consumidores(num_produtores, num_consumidores);
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 2:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Pedidos separados: %d\n", pedidos_produzidos_v2);
    printf("   Pedidos entregues: %d\n", pedidos_processados_v2);
    
    // ========== VERSÃO 3 ==========
    printf("\n EXECUTando VERSÃO 3: SEM CONTROLE (CONDIÇÕES DE CORRIDA)\n");
    start_time = get_timestamp();
    versao_sem_controle_pc(num_produtores, num_consumidores);
    end_time = get_timestamp();
    execution_time = end_time - start_time;
    
    printf("\n ESTATÍSTICAS DA VERSÃO 3:\n");
    printf("   Tempo total de execução: %.2f ms\n", execution_time);
    printf("   Pedidos separados: %d\n", pedidos_produzidos_v3);
    printf("   Pedidos entregues: %d\n", pedidos_processados_v3);
    printf("  Possíveis problemas: %d\n", 
           (pedidos_produzidos_v3 - pedidos_processados_v3));
    
    printf("\n RESUMO DO SISTEMA:\n");
    printf("   Versão 1: Controle com 1 entregador - Estável\n");
    printf("   Versão 2: Controle com vários entregadores - Eficiente\n");  
    printf("   Versão 3: Sem controle - Pode ter pedidos perdidos/duplicados\n");
    
    return 0;
}