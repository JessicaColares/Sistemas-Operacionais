#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    char url[100];
    char nome_aba[50];
    int id;
} aba_navegador;

typedef struct {
    char nome_arquivo[100];
    int tamanho_mb;
} download_info;

// Função para carregar página em uma thread
void *carregar_pagina_thread(void *arg) {
    aba_navegador *aba = (aba_navegador *)arg;
    int tempo = 2 + (rand() % 4); // Tempo aleatório entre 2-5 segundos
    
    printf("[THREAD %d - %s] Carregando: %s\n", aba->id, aba->nome_aba, aba->url);
    
    for (int i = 0; i < tempo; i++) {
        printf("[THREAD %d - %s] Progresso: %d/%d segundos\n", 
               aba->id, aba->nome_aba, i+1, tempo);
        sleep(1);
    }
    
    printf("[THREAD %d - %s] - Página completamente carregada!\n", 
           aba->id, aba->nome_aba);
    
    pthread_exit(NULL);
}

// Função para download em thread separada
void *download_thread(void *arg) {
    download_info *download = (download_info *)arg;
    
    printf("[DOWNLOAD THREAD] Iniciando: %s (%d MB)\n", 
           download->nome_arquivo, download->tamanho_mb);
    
    for (int i = 0; i < download->tamanho_mb; i++) {
        printf("[DOWNLOAD THREAD] %s: %d/%d MB\n", 
               download->nome_arquivo, i+1, download->tamanho_mb);
        sleep(1);
    }
    
    printf("[DOWNLOAD THREAD] - %s concluído!\n", download->nome_arquivo);
    
    pthread_exit(NULL);
}

// Thread da interface (simulando interação do usuário)
void *interface_thread(void *arg) {
    printf("\n[INTERFACE THREAD] Interface ativa e responsiva!\n");
    printf("[INTERFACE THREAD] Usuário pode:\n");
    
    for (int i = 1; i <= 10; i++) {
        printf("[INTERFACE THREAD] - Rolando página (%d/10)\n", i);
        printf("[INTERFACE THREAD] - Clicando em links\n");
        printf("[INTERFACE THREAD] - Digitando na barra de endereços\n");
        sleep(1);
    }
    
    printf("[INTERFACE THREAD] - Interface manteve responsividade durante todo o processo!\n");
    
    pthread_exit(NULL);
}

int main() {
    printf("================================\n");
    printf("NAVEGADOR WEB - VERSÃO COM THREADS\n");
    printf("================================\n\n");
    
    srand(time(NULL));
    
    // Usar clock_gettime para tempo real
    struct timespec inicio, fim;
    clock_gettime(CLOCK_REALTIME, &inicio);
    
    pthread_t threads_aba[3];
    pthread_t thread_download;
    pthread_t thread_interface;
    
    // Criar abas
    aba_navegador abas[3] = {
        {"https://www.google.com", "Google", 1},
        {"https://www.github.com", "GitHub", 2},
        {"https://www.youtube.com", "YouTube", 3}
    };
    
    download_info download = {"video.mp4", 5};
    
    printf("Usuário abre 3 abas simultaneamente...\n\n");
    
    // Lançar threads para cada aba
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads_aba[i], NULL, carregar_pagina_thread, &abas[i]);
    }
    
    // Lançar thread de download
    pthread_create(&thread_download, NULL, download_thread, &download);
    
    // Lançar thread de interface
    pthread_create(&thread_interface, NULL, interface_thread, NULL);
    
    // Aguardar todas as threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads_aba[i], NULL);
    }
    
    pthread_join(thread_download, NULL);
    pthread_join(thread_interface, NULL);
    
    clock_gettime(CLOCK_REALTIME, &fim);
    
    // Calcular tempo total em segundos
    double tempo_total = (fim.tv_sec - inicio.tv_sec) + 
                        (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    
    printf("\n================================\n");
    printf("TOTAL: Todas as operações concluídas em %.6f segundos\n", tempo_total);
    printf("================================\n");
    
    return 0;
}