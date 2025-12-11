#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void carregar_pagina(const char *url, int tempo_carregamento) {
    printf("[SEQUENCIAL] Iniciando carregamento: %s\n", url);
    sleep(tempo_carregamento); // Simula tempo de carregamento
    printf("[SEQUENCIAL] Página carregada: %s (tempo: %d segundos)\n", url, tempo_carregamento);
}

void download_arquivo(const char *arquivo, int tamanho_mb) {
    printf("[SEQUENCIAL] Iniciando download: %s (%d MB)\n", arquivo, tamanho_mb);
    sleep(tamanho_mb); // 1 segundo por MB
    printf("[SEQUENCIAL] Download completo: %s\n", arquivo);
}

int main() {
    printf("================================\n");
    printf("NAVEGADOR WEB - VERSÃO SEQUENCIAL\n");
    printf("================================\n\n");
    
    // Usar time() para tempo real (wall time)
    time_t inicio_real, fim_real;
    time(&inicio_real);
    
    // Simulação: usuário abre 3 abas
    printf("Usuário abre 3 abas simultaneamente...\n\n");
    
    // Aba 1
    printf("--- ABA 1 ---\n");
    carregar_pagina("https://www.google.com", 2);
    download_arquivo("documento.pdf", 3);
    
    // Aba 2 (só começa depois que aba 1 terminar)
    printf("\n--- ABA 2 ---\n");
    carregar_pagina("https://www.github.com", 3);
    download_arquivo("codigo.zip", 2);
    
    // Aba 3
    printf("\n--- ABA 3 ---\n");
    carregar_pagina("https://www.youtube.com", 4);
    download_arquivo("video.mp4", 5);
    
    time(&fim_real);
    double tempo_total = difftime(fim_real, inicio_real);
    
    printf("\n================================\n");
    printf("TOTAL: Todas as abas processadas em %.6f segundos\n", tempo_total);
    printf("================================\n");
    
    return 0;
}