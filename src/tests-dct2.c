#include "dct2.h"
#include "fftw-dct2.h"

#include <stdio.h>
#ifdef _WIN32
#include <malloc.h>
#else
#include <stdlib.h>
#include <memory.h>
#endif

#include <math.h>

#define N 8


int main(void) {
    dct_context* ctx = dct_context_alloc(N * N);

    double matrix[N * N] = {
        231, 32, 233, 161, 24, 71, 140, 245,
        247, 40, 248, 245, 124, 204, 36, 107,
        234, 202, 245, 167, 9, 217, 239, 173,
        193, 190, 100, 167, 43, 180, 8, 70,
        11, 24, 210, 177, 81, 243, 8, 112,
        97, 195, 203, 47, 125, 114, 165, 181,
        193, 70, 174, 167, 41, 30, 127, 245,
        87, 149, 57, 192, 65, 129, 178, 228
    };

    double expected_matrix[N * N] = {
        1.11e+03, 4.40e+01, 7.59e+01, -1.38e+02, 3.50e+00, 1.22e+02, 1.95e+02, -1.01e+02,
        7.71e+01, 1.14e+02, -2.18e+01, 4.13e+01, 8.77e+00, 9.90e+01, 1.38e+02, 1.09e+01,
        4.48e+01, -6.27e+01, 1.11e+02, -7.63e+01, 1.24e+02, 9.55e+01, -3.98e+01, 5.85e+01,
        -6.99e+01, -4.02e+01, -2.34e+01, -7.67e+01, 2.66e+01, -3.68e+01, 6.61e+01, 1.25e+02,
        -1.09e+02, -4.33e+01, -5.55e+01, 8.17e+00, 3.02e+01, -2.86e+01, 2.44e+00, -9.41e+01,
        -5.38e+00, 5.66e+01, 1.73e+02, -3.54e+01, 3.23e+01, 3.34e+01, -5.81e+01, 1.90e+01,
        7.88e+01, -6.45e+01, 1.18e+02, -1.50e+01, -1.37e+02, -3.06e+01, -1.05e+02, 3.98e+01,
        1.97e+01, -7.81e+01, 9.72e-01, -7.23e+01, -2.15e+01, 8.13e+01, 6.37e+01, 5.90e+0
    };

    // Esegui la DCT2 Nostra
    printf("Eseguendo la DCT2 Nostra...\n");
    double* matrix_out = (double*)malloc(N * N * sizeof(double));
    memcpy(matrix_out, matrix, N * N * sizeof(double));
    int result = dct2d(ctx, matrix_out, N, N);

    if (result != 0) {
        printf("Errore durante la DCT2 Nostra\n");
        free(matrix_out);
        dct_context_free(ctx);
        return -1;
    }

    printf("DCT2 Nostra completata!\n");
    printf("Comparando i risultati...\n");
    // Confronta i risultati
    for (int i = 0; i < N * N; i++) {
        if (fabs(matrix_out[i] - expected_matrix[i]) > 10) {
            printf("Errore alla posizione [%d][%d]: atteso %f, ottenuto %f\n", (int)(i / 8), i % 8, expected_matrix[i], matrix_out[i]);
            free(matrix_out);
            dct_context_free(ctx);
            return -1;
        }
    }

    printf("DCT2 Nostra ha passato il test!\n");

    // Esegui la iDCT2 Nostra
    printf("Eseguendo la iDCT2 Nostra...\n");
    result = idct2d(ctx, matrix_out, N, N);

    if (result != 0) {
        printf("Errore durante la iDCT2 Nostra\n");
        free(matrix_out);
        dct_context_free(ctx);
        return -1;
    }

    // Confronta i risultati
    printf("Comparando i risultati...\n");
    for (int i = 0; i < N * N; i++) {
        if (fabs(matrix_out[i] - matrix[i]) > 1e-5) {
            printf("Errore alla posizione %d: atteso %f, ottenuto %f\n", i, matrix[i], matrix_out[i]);
            free(matrix_out);
            dct_context_free(ctx);
            return -1;
        }
    }

    free(matrix_out);
    dct_context_free(ctx);
    printf("IDCT2 Nostra ha passato il test!\n");

    // Esegui la DCT2 FFTW
    printf("Eseguendo la DCT2 FFTW...\n");
    double* matrix_fftw_out = (double*)malloc(N * N * sizeof(double));
    memcpy(matrix_fftw_out, matrix, N * N * sizeof(double));

    compute_fftw_dct2d(matrix, matrix_fftw_out, N, N);

    // Confronta i risultati
    printf("Comparando i risultati...\n");
    for (int i = 0; i < N * N; i++) {
        if (fabs(matrix_fftw_out[i] - expected_matrix[i]) > 10) {
            printf("Errore alla posizione %d: atteso %f, ottenuto %f\n", i, expected_matrix[i], matrix_fftw_out[i]);
            free(matrix_fftw_out);
            return -1;
        }
    }

    printf("DCT2 FFTW completata!\n");
    // Esegui la iDCT2 FFTW
    printf("Eseguendo la iDCT2 FFTW...\n");
    double* matrix_fftw_out2 = (double*)malloc(N * N * sizeof(double));
    memcpy(matrix_fftw_out2, matrix, N * N * sizeof(double));
    compute_fftw_idct2d(matrix_fftw_out, matrix_fftw_out2, N, N);

    if (result != 0) {
        printf("Errore durante la iDCT2 FFTW\n");
        free(matrix_fftw_out);
        free(matrix_fftw_out2);
        return -1;
    }

    // Confronta i risultati
    printf("Comparando i risultati...\n");
    for (int i = 0; i < N * N; i++) {
        if (fabs(matrix_fftw_out2[i] - matrix[i]) > 1e-1) {
            printf("Errore alla posizione %d: atteso %f, ottenuto %f\n", i, matrix[i], matrix_fftw_out2[i]);
            free(matrix_fftw_out);
            free(matrix_fftw_out2);
            return -1;
        }
    }

    printf("Comparazione iDCT2 FFTW completata!\n");
    
    free(matrix_fftw_out);
    free(matrix_fftw_out2);

    printf("Tutti i test sono stati superati con successo!\n");
    return 0;
}
