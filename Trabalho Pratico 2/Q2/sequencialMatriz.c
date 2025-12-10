#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_multiply_seq(double **A, double **B, double **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamanho_da_matriz>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    // Alocação dinâmica das matrizes
    double **A = (double **)malloc(n * sizeof(double *));
    double **B = (double **)malloc(n * sizeof(double *));
    double **C = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        B[i] = (double *)malloc(n * sizeof(double));
        C[i] = (double *)malloc(n * sizeof(double));
    }

    // Inicialização com valores simples
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = 1.0;
            B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    clock_t start = clock();
    matrix_multiply_seq(A, B, C, n);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Verificação simples (apenas um elemento)
    printf("================================\n");
    printf("MULTIPLICAÇÃO DE MATRIZES SEQUENCIAL\n");
    printf("================================\n");
    printf("Tamanho da matriz: %dx%d\n", n, n);
    printf("C[0][0] = %.2f (elemento de teste)\n", C[0][0]);
    printf("Tempo de execução: %.6f segundos\n", time_spent);
    printf("================================\n");

    // Liberação de memória
    for (int i = 0; i < n; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}