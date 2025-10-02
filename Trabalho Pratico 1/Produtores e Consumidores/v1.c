#include "loja.h"
#include "v1.h"

// Variáveis para estatísticas
int pedidos_produzidos_v1 = 0;
int pedidos_processados_v1 = 0;
int produtores_ativos = 0;

void* produtor_v1(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'P', "Iniciada - Funcionário separação");
    
    // Cada produtor cria 3 pedidos
    for (int i = 0; i < 3; i++) {
        log_estado(id, 'P', "Aguardando espaço no buffer para novo pedido");
        
        // Espera por espaço vazio no buffer
        sem_wait(&vazio);
        sem_wait(&mutex);
        
        // CRIAR PEDIDO (Região Crítica)
        Pedido novo_pedido;
        novo_pedido.id_pedido = pedido_id_counter++;
        strcpy(novo_pedido.etapa, "em separacao");
        
        // Adiciona ao buffer
        buffer.pedidos[buffer.fim] = novo_pedido;
        buffer.fim = (buffer.fim + 1) % BUFFER_SIZE;
        buffer.count++;
        
        pedidos_produzidos_v1++;
        
        log_operacao(id, 'P', "Separou", novo_pedido.id_pedido, "em separacao");
        
        // Libera o buffer
        sem_post(&mutex);
        sem_post(&cheio);
        
        // Simula tempo de separação do pedido
        usleep(args->sleep_time * 1000);
    }
    
    // Marca que este produtor terminou
    sem_wait(&mutex);
    produtores_ativos--;
    sem_post(&mutex);
    
    log_estado(id, 'P', "Finalizada - Funcionário separação");
    free(args);
    return NULL;
}

void* consumidor_v1(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'C', "Iniciada - Funcionário entrega");
    
    // Continua enquanto houver produtores ativos OU pedidos no buffer
    while (1) {
        sem_wait(&mutex);
        int nao_ha_mais_trabalho = (produtores_ativos == 0 && buffer.count == 0);
        sem_post(&mutex);
        
        if (nao_ha_mais_trabalho) {
            break;
        }
        
        log_estado(id, 'C', "Aguardando pedidos para processar");
        
        // Tenta pegar um pedido com timeout para evitar deadlock
        if (sem_trywait(&cheio) == 0) {
            sem_wait(&mutex);
            
            if (buffer.count > 0) {
                // Pega o próximo pedido
                Pedido pedido = buffer.pedidos[buffer.inicio];
                buffer.inicio = (buffer.inicio + 1) % BUFFER_SIZE;
                buffer.count--;
                
                // Avança para a próxima etapa
                const char* nova_etapa = proxima_etapa(pedido.etapa);
                
                if (strcmp(nova_etapa, "entregue") == 0) {
                    // Pedido finalizado
                    pedidos_processados_v1++;
                    log_operacao(id, 'C', "Entregou pedido", pedido.id_pedido, "entregue");
                    
                    // Libera espaço no buffer
                    sem_post(&vazio);
                } else {
                    // Recoloca no buffer com nova etapa
                    strcpy(pedido.etapa, nova_etapa);
                    buffer.pedidos[buffer.fim] = pedido;
                    buffer.fim = (buffer.fim + 1) % BUFFER_SIZE;
                    buffer.count++;
                    
                    log_operacao(id, 'C', "Processou pedido", pedido.id_pedido, nova_etapa);
                    
                    // Libera para outros consumidores
                    sem_post(&cheio);
                }
            }
            
            sem_post(&mutex);
        } else {
            // Não há pedidos disponíveis no momento
            usleep(100000); // Espera 100ms e tenta novamente
            continue;
        }
        
        // Simula tempo de processamento
        usleep(args->sleep_time * 1000);
    }
    
    log_estado(id, 'C', "Finalizada - Funcionário entrega");
    free(args);
    return NULL;
}

void versao_1_consumidor(int num_produtores, int num_consumidores) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("VERSÃO 1 - SISTEMA DE ENTREGAS\n");
    printf("%d FUNCIONÁRIOS SEPARANDO + 1 FUNCIONÁRIO ENTREGANDO\n", num_produtores);
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("═══════════════════════════════════════════════════\n\n");
    
    // Reinicializa contadores e buffer
    inicializar_buffer();
    pedidos_produzidos_v1 = 0;
    pedidos_processados_v1 = 0;
    produtores_ativos = num_produtores;
    
    // Inicializa semáforos
    inicializar_semaforos();
    
    pthread_t threads[num_produtores + num_consumidores];
    int thread_index = 0;
    
    // Cria threads produtoras (funcionários separando pedidos)
    for (int i = 0; i < num_produtores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'P';
        args->sleep_time = 200 + (rand() % 300); // 200-500ms
        
        pthread_create(&threads[thread_index++], NULL, produtor_v1, args);
    }
    
    // Cria threads consumidoras (funcionários processando entregas)
    for (int i = 0; i < num_consumidores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'C';
        args->sleep_time = 300 + (rand() % 400); // 300-700ms
        
        pthread_create(&threads[thread_index++], NULL, consumidor_v1, args);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_produtores + num_consumidores; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n═══════════════════════════════════════════════════\n");
    printf("FIM DA EXECUÇÃO - TODOS OS PEDIDOS PROCESSADOS\n");
    printf("Pedidos separados: %d, Pedidos entregues: %d\n", pedidos_produzidos_v1, pedidos_processados_v1);
    printf("═══════════════════════════════════════════════════\n");
    
    destruir_semaforos();
}