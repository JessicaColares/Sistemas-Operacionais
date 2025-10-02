#include "loja.h"
#include "v2.h"

// Variáveis para estatísticas
int pedidos_produzidos_v2 = 0;
int pedidos_processados_v2 = 0;
int produtores_ativos_v2 = 0;  // Nova variável para controlar produtores ativos

void* produtor_v2(void* arg) {
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
        
        pedidos_produzidos_v2++;
        
        log_operacao(id, 'P', "Separou pedido", novo_pedido.id_pedido, "em separacao");
        
        // Libera o buffer
        sem_post(&mutex);
        sem_post(&cheio);
        
        // Simula tempo de separação do pedido
        usleep(args->sleep_time * 1000);
    }
    
    // Marca que este produtor terminou
    sem_wait(&mutex);
    produtores_ativos_v2--;
    sem_post(&mutex);
    
    log_estado(id, 'P', "Finalizada - Funcionário separação");
    free(args);
    return NULL;
}

void* consumidor_v2(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    int id = args->id;
    
    log_estado(id, 'C', "Iniciada - Funcionário entrega");
    
    while (1) {
        int deve_terminar = 0;
        
        // Verifica se deve terminar (com proteção do mutex)
        sem_wait(&mutex);
        if (produtores_ativos_v2 == 0 && buffer.count == 0) {
            deve_terminar = 1;
        }
        sem_post(&mutex);
        
        if (deve_terminar) {
            break;
        }
        
        log_estado(id, 'C', "Aguardando pedidos para processar entrega");
        
        // Tenta pegar um pedido com timeout para evitar deadlock
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_nsec += 100000000; // Timeout de 100ms
        if (ts.tv_nsec >= 1000000000) {
            ts.tv_sec += 1;
            ts.tv_nsec -= 1000000000;
        }
        
        if (sem_timedwait(&cheio, &ts) == 0) {
            sem_wait(&mutex);
            
            if (buffer.count > 0) {
                Pedido pedido = buffer.pedidos[buffer.inicio];
                
                // Avança para a próxima etapa
                const char* nova_etapa = proxima_etapa(pedido.etapa);
                strcpy(pedido.etapa, nova_etapa);
                
                // Se chegou na última etapa, remove do buffer
                if (strcmp(nova_etapa, "entregue") == 0) {
                    buffer.inicio = (buffer.inicio + 1) % BUFFER_SIZE;
                    buffer.count--;
                    pedidos_processados_v2++;
                    
                    log_operacao(id, 'C', "Entregou pedido", pedido.id_pedido, "entregue");
                    
                    // Libera espaço no buffer
                    sem_post(&vazio);
                } else {
                    // Atualiza no buffer (ainda não finalizou)
                    buffer.pedidos[buffer.inicio] = pedido;
                    log_operacao(id, 'C', "Atualizou pedido", pedido.id_pedido, nova_etapa);
                    
                    // Libera para outros consumidores
                    sem_post(&cheio);
                }
            }
            
            sem_post(&mutex);
        } else {
            // Timeout - verifica novamente se deve terminar
            continue;
        }
        
        // Simula tempo de processamento da entrega
        usleep(args->sleep_time * 1000);
    }
    
    log_estado(id, 'C', "Finalizada - Funcionário entrega");
    free(args);
    return NULL;
}

void versao_varios_consumidores(int num_produtores, int num_consumidores) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("VERSÃO 2 - SISTEMA DE ENTREGAS\n");
    printf("%d FUNCIONÁRIOS SEPARANDO + %d FUNCIONÁRIOS ENTREGANDO\n", num_produtores, num_consumidores);
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("═══════════════════════════════════════════════════\n\n");
    
    // Reinicializa contadores e buffer
    inicializar_buffer();
    pedidos_produzidos_v2 = 0;
    pedidos_processados_v2 = 0;
    produtores_ativos_v2 = num_produtores;  // Inicializa contador de produtores ativos
    
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
        
        pthread_create(&threads[thread_index++], NULL, produtor_v2, args);
    }
    
    // Cria threads consumidoras (funcionários processando entregas)
    for (int i = 0; i < num_consumidores; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;
        args->tipo = 'C';
        args->sleep_time = 300 + (rand() % 400); // 300-700ms
        
        pthread_create(&threads[thread_index++], NULL, consumidor_v2, args);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < num_produtores + num_consumidores; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n═══════════════════════════════════════════════════\n");
    printf("FIM DA EXECUÇÃO - TODOS OS PEDIDOS PROCESSADOS\n");
    printf("═══════════════════════════════════════════════════\n");
    
    destruir_semaforos();
}