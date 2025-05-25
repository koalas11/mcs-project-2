#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include <math.h>


void normalize_fftw_2d(double* data, int width, int height) {
    double scale = 4 * width * height;
    for (int i = 0; i < width * height; i++) {
        data[i] = data[i] / scale;
    }
}

void normalize_dct2_ortho(double* data, int width, int height) {
    for (int i = 0; i < height; i++) {
        double ai = (i == 0 ? 1.0 / sqrt(4.0 * height) : 1.0 / sqrt(2.0 * height));
        for (int j = 0; j < width; j++) {
            double aj = (j == 0 ? 1.0 / sqrt(4.0 * width) : 1.0 / sqrt(2.0 * width));
            data[i * width + j] *= ai * aj;
        }
    }
}

void normalize_idct2_ortho(double* data, int width, int height) {
    for (int i = 0; i < height; i++) {
        double ai = (i == 0 ? 1.0 / sqrt(4.0 * height) : 1.0 / sqrt(2.0 * height));
        for (int j = 0; j < width; j++) {
            double aj = (j == 0 ? 1.0 / sqrt(4.0 * width) : 1.0 / sqrt(2.0 * width));
            data[i * width + j] /= ai * aj;
        }
    }
}

void compute_fftw_dct2d(double* in, double* out, int width, int height) {
    fftw_plan plan;

    // Create FFTW plan for 2D DCT-II
    plan = fftw_plan_r2r_2d(width, height, in, out, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE | FFTW_PATIENT);

    // Execute the transform
    fftw_execute(plan);

    // Clean up
    fftw_destroy_plan(plan);
    fftw_cleanup();

    normalize_dct2_ortho(out, width, height);
}

void compute_fftw_idct2d(double* in, double* out, int width, int height) {
    fftw_plan plan;

    normalize_idct2_ortho(in, width, height);

    // Create FFTW plan for 2D DCT-III
    plan = fftw_plan_r2r_2d(width, height, in, out, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE | FFTW_PATIENT);

    // Execute the transform
    fftw_execute(plan);

    // Clean up
    fftw_destroy_plan(plan);
    fftw_cleanup();

    // normalize_idct2_ortho(out, width, height);
    normalize_fftw_2d(out, width, height);
}
