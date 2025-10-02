#include "loja.h"
#include "v3.h"

// Variáveis para estatísticas
int pedidos_produzidos_v3 = 0;
int pedidos_processados_v3 = 0;

void* produtor_sem_controle(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'P', "Iniciada - Funcionário separação (SEM CONTROLE)");
    
    // Cada produtor cria 3 pedidos
    for (int i = 0; i < 3; i++) {
        log_estado(id, 'P', "Separando pedido (SEM CONTROLE)");
        
        // CRIAR PEDIDO - SEM CONTROLE DE CONCORRÊNCIA
        // PODE OCORRER CONDIÇÃO DE CORRIDA!
        
        // Verifica se há espaço (sem sincronização)
        if (buffer.count < BUFFER_SIZE) {
            Pedido novo_pedido;
            novo_pedido.id_pedido = pedido_id_counter++;
            strcpy(novo_pedido.etapa, "em separacao");
            
            // Adiciona ao buffer (sem proteção)
            buffer.pedidos[buffer.fim] = novo_pedido;
            buffer.fim = (buffer.fim + 1) % BUFFER_SIZE;
            buffer.count++;
            
            pedidos_produzidos_v3++;
            
            log_operacao(id, 'P', "Separou pedido", novo_pedido.id_pedido, "em separacao");
        } else {
            log_estado(id, 'P', "Buffer cheio - perdendo pedido (SEM CONTROLE)");
        }
        
        // Simula tempo de separação
        usleep(args->sleep_time * 1000);
    }
    
    log_estado(id, 'P', "Finalizada - Funcionário separação (SEM CONTROLE)");
    free(args);
    return NULL;
}

void* consumidor_sem_controle(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    int tentativas_vazias = 0;
    const int MAX_TENTATIVAS_VAZIAS = 10; // Para evitar loop infinito
    
    log_estado(id, 'C', "Iniciada - Funcionário entrega (SEM CONTROLE)");
    
    // Continua processando até que não haja mais trabalho
    while (tentativas_vazias < MAX_TENTATIVAS_VAZIAS) {
        log_estado(id, 'C', "Tentando processar entrega (SEM CONTROLE)");
        
        // PROCESSAR PEDIDO - SEM CONTROLE DE CONCORRÊNCIA
        // PODE OCORRER CONDIÇÃO DE CORRIDA!
        
        // Verifica se há pedidos (sem sincronização)
        if (buffer.count > 0) {
            Pedido pedido = buffer.pedidos[buffer.inicio];
            
            // Avança para a próxima etapa
            const char* nova_etapa = proxima_etapa(pedido.etapa);
            strcpy(pedido.etapa, nova_etapa);
            
            // Se chegou na última etapa, remove do buffer
            if (strcmp(nova_etapa, "entregue") == 0) {
                buffer.inicio = (buffer.inicio + 1) % BUFFER_SIZE;
                buffer.count--;
                pedidos_processados_v3++;
                
                log_operacao(id, 'C', "Entregou pedido", pedido.id_pedido, "entregue");
                tentativas_vazias = 0; // Reset contador
            } else {
                // Atualiza no buffer (ainda não finalizou)
                buffer.pedidos[buffer.inicio] = pedido;
                log_operacao(id, 'C', "Atualizou pedido", pedido.id_pedido, nova_etapa);
                tentativas_vazias = 0; // Reset contador
            }
        } else {
            log_estado(id, 'C', "Buffer vazio - nada para processar (SEM CONTROLE)");
            tentativas_vazias++;
        }
        
        // Simula tempo de processamento
        usleep(args->sleep_time * 1000);
    }
    
    log_estado(id, 'C', "Finalizada - Funcionário entrega (SEM CONTROLE)");
    free(args);
    return NULL;
}

void versao_sem_controle_pc(int num_produtores, int num_consumidores) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("VERSÃO 3 - SISTEMA DE ENTREGAS (SEM CONTROLE)\n");
    printf("%d FUNCIONÁRIOS SEPARANDO + %d FUNCIONÁRIOS ENTREGANDO\n", num_produtores, num_consumidores);
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("  PODE OCORRER CONDIÇÕES DE CORRIDA!\n");
    printf("═══════════════════════════════════════════════════\n\n");
    
    // Reinicializa contadores e buffer
    inicializar_buffer();
    pedidos_produzidos_v3 = 0;
    pedidos_processados_v3 = 0;
    
    pthread_t threads[num_produtores + num_consumidores];
    int thread_index = 0;
    
    // Cria threads produtoras
    for (int i = 0; i < num_produtores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'P';
        args->sleep_time = 200 + (rand() % 300); // 200-500ms
        
        pthread_create(&threads[thread_index++], NULL, produtor_sem_controle, args);
    }
    
    // Cria threads consumidoras
    for (int i = 0; i < num_consumidores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'C';
        args->sleep_time = 300 + (rand() % 400); // 300-700ms
        
        pthread_create(&threads[thread_index++], NULL, consumidor_sem_controle, args);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_produtores + num_consumidores; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n═══════════════════════════════════════════════════\n");
    printf("FIM DA EXECUÇÃO - SISTEMA SEM CONTROLE\n");
    printf("═══════════════════════════════════════════════════\n");
}