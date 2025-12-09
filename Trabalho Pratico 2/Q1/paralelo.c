#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    double *vec1;
    double *vec2;
    int start;
    int end;
    double partial_sum;
} thread_data;

void *dot_product_parallel(void *arg) {
    thread_data *data = (thread_data *)arg;
    double sum = 0.0;
    for (int i = data->start; i < data->end; i++) {
        sum += data->vec1[i] * data->vec2[i];
    }
    data->partial_sum = sum;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <tamanho_do_vetor> <numero_de_threads>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    double *vec1 = (double *)malloc(size * sizeof(double));
    double *vec2 = (double *)malloc(size * sizeof(double));

    for (int i = 0; i < size; i++) {
        vec1[i] = 1.0;
        vec2[i] = 1.0;
    }

    pthread_t threads[num_threads];
    thread_data tdata[num_threads];
    int chunk_size = size / num_threads;

    clock_t start = clock();

    for (int i = 0; i < num_threads; i++) {
        tdata[i].vec1 = vec1;
        tdata[i].vec2 = vec2;
        tdata[i].start = i * chunk_size;
        tdata[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, dot_product_parallel, &tdata[i]);
    }

    double total_sum = 0.0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += tdata[i].partial_sum;
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("================================\n");
    printf("PRODUTO ESCALAR PARALELO\n");
    printf("================================\n");
    printf("Tamanho do vetor: %d\n", size);
    printf("Número de threads: %d\n", num_threads);
    printf("Tempo de execução: %.6f segundos\n", time_spent);
    printf("Tamanho do pedaço (chunk) por thread: %d elementos\n", chunk_size);
    printf("================================\n");

    free(vec1);
    free(vec2);

    return 0;
}