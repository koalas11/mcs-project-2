#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dct2.h"
#include "fftw-dct2.h"

#define MIN_N 2
#define MAX_N 4096
#define REPEAT 5

// Alloca una matrice contigua e restituisce il puntatore al blocco
double *alloc_matrix_contiguous(int n) {
    return (double *) malloc(n * n * sizeof(double));
}

// Riempie una matrice contigua con valori random
void fill_random_contiguous(double *mat, int n) {
    for (int i = 0; i < n * n; i++)
        mat[i] = (double)rand() / RAND_MAX;
}

int main() {
    FILE *csv = fopen("bench.csv", "w");
    if (!csv) {
        perror("Cannot open CSV file");
        return 1;
    }
    fprintf(csv, "n,dct2d_ms,idct2d_ms,fftw_dct2d_ms,fftw_idct2d_ms\n");

    srand((unsigned int)time(NULL));

    for (int n = MIN_N; n <= MAX_N; n = n * 2) {
        printf("Eseguendo benchmark per n = %d...\n", n);
        // Matrici contigue per la nostra DCT2
        double *in_c = alloc_matrix_contiguous(n);
        double *out_c = alloc_matrix_contiguous(n);

        double total_dct2d = 0, total_idct2d = 0, total_fftw_dct2d = 0, total_fftw_idct2d = 0;

        dct_context *ctx = dct_context_alloc(n);

        for (int rep = 0; rep < REPEAT; rep++) {
            fill_random_contiguous(in_c, n);

            // Benchmark nostra DCT2 (usa double*)
            clock_t start = clock();
            dct2d(ctx, in_c, n, n);
            clock_t end = clock();
            total_dct2d += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            start = clock();
            idct2d(ctx, in_c, n, n);
            end = clock();
            total_idct2d += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            // Benchmark FFTW DCT2 (usa double**)
            start = clock();
            compute_fftw_dct2d(in_c, out_c, n, n);
            end = clock();
            total_fftw_dct2d += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

            // Benchmark FFTW IDCT2 (se esiste, qui ripetuto come esempio)
            start = clock();
            compute_fftw_idct2d(in_c, out_c, n, n);
            end = clock();
            total_fftw_idct2d += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        }

        dct_context_free(ctx);

        fprintf(csv, "%d,%.3f,%.3f,%.3f,%.3f\n", n, total_dct2d / REPEAT, total_idct2d / REPEAT, total_fftw_dct2d / REPEAT, total_fftw_idct2d / REPEAT);

        free(in_c);
        free(out_c);
        printf("DCT2D e IDCT2D per n = %d completati.\n", n);
    }

    fclose(csv);
    printf("Benchmark completato. Risultati in bench.csv\n");
    return 0;
}
