#ifndef BANCARIO_H
#define BANCARIO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// ==================== ESTRUTURAS DE DADOS ====================
typedef struct {
    double saldo;
    int ultima_transacao;
    char historico[100];
} ContaBancaria;

typedef struct {
    int id;
    char tipo;  // 'L' para leitor, 'E' para escritor
    double valor; // valor para depósito/saque
    int sleep_time; // tempo de sleep em ms
} ThreadArgs;

// ==================== VARIÁVEIS GLOBAIS ====================
extern ContaBancaria conta;
extern sem_t mutex, recurso;
extern int leitores_ativos;

// ==================== VARIÁVEIS DE ESTATÍSTICA ==============
extern int leituras_realizadas;
extern int escritas_realizadas;

// ==================== FUNÇÕES AUXILIARES ====================
void inicializar_conta();
void inicializar_semaforos();
void destruir_semaforos();
void log_operacao(int id, char tipo, const char* operacao, double valor, double novo_saldo, const char* observacao);
void log_estado(int id, char tipo, const char* estado);

// ==================== FUNÇÕES DAS THREADS ====================
void* leitor(void* arg);
void* escritor(void* arg);

#endif