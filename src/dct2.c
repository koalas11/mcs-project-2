#include "dct2.h"

// #include <gsl/gsl_integration.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#undef M_PI
#define M_PI 3.14159265358979323846264338327950288

typedef struct dct_context {} dct_context;

dct_context* dct_context_alloc() {
  return malloc(sizeof(dct_context));
}
void dct_context_free(dct_context* ctx) {
  free(ctx);
}

int dct_init(dct_context* ctx) {
  return 0;
}
int dct_shutdown(dct_context* ctx) {
  return 0;
}

int dct1d(double* base, size_t length, size_t stride, double* out) {
  // double* c = malloc(length * sizeof(double));
  if (!out) return -1;
  if (stride <= 0) return -1;

  for (size_t k = 0; k < length; ++k) {
    double s = 0;
    double alpha = (k == 0) ? 1.0 / sqrt((double)length) : sqrt(2.0 / length);

    for (size_t i = 0; i < length; ++i)
      s += base[i * stride] * cos(k * M_PI * ((2.0 * i + 1) / (2.0 * length)));

    out[k] = alpha * s;
  }

  // for (size_t i = 0; i < length; ++i)
  //   base[i * stride] = c[i];

  // free(c);
  return 0;
}

int idct1d(double* vector, size_t length, size_t stride, double* out) {
  // double* f = malloc(length * sizeof(double));
  if (!out) return -1;
  if (stride <= 0) return -1;

  for (size_t i = 0; i < length; ++i) {
    double s = 0;

    for (size_t k = 0; k < length; ++k) {
      double alpha = (k == 0) ? 1.0 / sqrt((double)length) : sqrt(2.0 / length);

      s += vector[k * stride] * alpha * cos(k * M_PI * ((2.0 * i + 1) / (2.0 * length)));
    }

    out[i] = s;
  }

  // memcpy(f, vector, length * sizeof(double));
  // free(f);

  return 0;
}

int dct2d(double* matrix, size_t width, size_t height) {
  double* temp = (double*)malloc((width > height ? width : height) * sizeof(double));
  if (!temp) return -1;

  for (size_t row = 0; row < height; ++row) {
    dct1d(&matrix[row * width], width, 1, temp);
    for (size_t i = 0; i < width; ++i)
      matrix[row * width + i] = temp[i];
  }

  for (size_t col = 0; col < width; ++col) {
    dct1d(&matrix[col], height, width, temp);
    for (size_t i = 0; i < height; ++i)
      matrix[i * width + col] = temp[i];
  }

  free(temp);

  return 0;
}

int idct2d(double* matrix, size_t width, size_t height) {
  double* temp = (double*)malloc((width > height ? width : height) * sizeof(double));
  if (!temp) return -1;

  for (size_t row = 0; row < height; ++row) {
    idct1d(&matrix[row * width], width, 1, temp);
    for (size_t i = 0; i < width; ++i)
      matrix[row * width + i] = temp[i];
  }

  for (size_t col = 0; col < width; ++col) {
    idct1d(&matrix[col], height, width, temp);
    for (size_t i = 0; i < height; ++i)
      matrix[i * width + col] = temp[i];
  }

  free(temp);

  return 0;
}
