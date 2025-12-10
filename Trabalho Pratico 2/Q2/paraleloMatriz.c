#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    double **A;
    double **B;
    double **C;
    int n;
    int start_row;
    int end_row;
} thread_data;

void *matrix_multiply_parallel(void *arg) {
    thread_data *data = (thread_data *)arg;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < data->n; j++) {
            double sum = 0.0;
            for (int k = 0; k < data->n; k++) {
                sum += data->A[i][k] * data->B[k][j];
            }
            data->C[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <tamanho_da_matriz> <numero_de_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    // Alocação dinâmica
    double **A = (double **)malloc(n * sizeof(double *));
    double **B = (double **)malloc(n * sizeof(double *));
    double **C = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        B[i] = (double *)malloc(n * sizeof(double));
        C[i] = (double *)malloc(n * sizeof(double));
    }

    // Inicialização
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = 1.0;
            B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    pthread_t threads[num_threads];
    thread_data tdata[num_threads];
    int rows_per_thread = n / num_threads;

    clock_t start = clock();

    for (int i = 0; i < num_threads; i++) {
        tdata[i].A = A;
        tdata[i].B = B;
        tdata[i].C = C;
        tdata[i].n = n;
        tdata[i].start_row = i * rows_per_thread;
        tdata[i].end_row = (i == num_threads - 1) ? n : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, matrix_multiply_parallel, &tdata[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("================================\n");
    printf("MULTIPLICAÇÃO DE MATRIZES PARALELA\n");
    printf("================================\n");
    printf("Tamanho da matriz: %dx%d\n", n, n);
    printf("Número de threads: %d\n", num_threads);
    printf("Linhas por thread: %d\n", rows_per_thread);
    printf("C[0][0] = %.2f (elemento de teste)\n", C[0][0]);
    printf("Tempo de execução: %.6f segundos\n", time_spent);
    printf("================================\n");

    // Liberação
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