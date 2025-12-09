#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double dot_product_seq(double *vec1, double *vec2, int size) {
    double result = 0.0;
    for (int i = 0; i < size; i++) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamanho_do_vetor>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    double *vec1 = (double *)malloc(size * sizeof(double));
    double *vec2 = (double *)malloc(size * sizeof(double));

    // Inicializa vetores com valores simples
    for (int i = 0; i < size; i++) {
        vec1[i] = 1.0;
        vec2[i] = 1.0;
    }

    clock_t start = clock();
    double result = dot_product_seq(vec1, vec2, size);
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("================================\n");
    printf("PRODUTO ESCALAR SEQUENCIAL\n");
    printf("================================\n");
    printf("Tamanho do vetor: %d\n", size);
    printf("Tempo de execução: %.6f segundos\n", time_spent);
    printf("================================\n");

    free(vec1);
    free(vec2);

    return 0;
}