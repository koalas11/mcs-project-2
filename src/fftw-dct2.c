#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>


void normalize(double *data, int width, int height) {
    double scale = 3.0;
    for (int i = 0; i < width * height; i++) {
        data[i] = (int) (data[i] * scale);
    }
}

void normalize_dct2_ortho(double *data, int width, int height) {
    for (int k1 = 0; k1 < height; ++k1) { // righe
        double alpha1 = (k1 == 0) ? 1.0 / sqrt(4 * height) : 1.0 / sqrt(2.0 * height);
        for (int k2 = 0; k2 < width; ++k2) { // colonne
            double alpha2 = (k2 == 0) ? 1.0 / sqrt(4 * width) : 1.0 / sqrt(2.0 * width);
            data[k1 * width + k2] *= alpha1 * alpha2;
        }
    }
}

void compute_fftw_dct2d(double* in, double* out, int width, int height) {
    fftw_plan plan;

    // Create FFTW plan for 2D DCT-II
    plan = fftw_plan_r2r_2d(width, height, in, out, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);

    // Execute the transform
    fftw_execute(plan);

    normalize_dct2_ortho(out, width, height);

    // Clean up
    fftw_destroy_plan(plan);
    fftw_cleanup();
}

void compute_fftw_idct2d(double* in, double* out, int width, int height) {
    fftw_plan plan;

    // Create FFTW plan for 2D iDCT-II
    plan = fftw_plan_r2r_2d(width, height, in, out, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);

    // Execute the transform
    fftw_execute(plan);

    // Normalize the output
    //normalize(out, width, height);
    
    // Clean up
    fftw_destroy_plan(plan);
    fftw_cleanup();
}
