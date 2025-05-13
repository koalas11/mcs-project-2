#include "dct2.h"

// #include <gsl/gsl_integration.h>
#include <assert.h>
#include <string.h>
#include <math.h>

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

int dct1d(double* vector, size_t length) {
  double* c = malloc(length * sizeof(double));

  for (size_t k = 0; k < length; ++k) {
    double s = 0;
    double alpha;
    if (k == 0)
      alpha = 1 / sqrt(length);
    else
      alpha = sqrt(2 / length);

    for (size_t i = 0; i < length; ++i)
      s += vector[i] * cos(k * M_PI * ((2 * i + 1) / (2 * length)));

    c[k] = alpha * s;
  }
  memcpy(c, vector, sizeof(c));
  free(c);

  return 0;
}

int idct1d(double* vector, size_t length) {
  double* f = malloc(length * sizeof(vector));

  for (size_t i = 0; i < length; ++i) {
    double s = 0;

    for (size_t k = 0; k < length; ++k) {
      double alpha;
      if (k == 0)
        alpha = 1 / sqrt(length);
      else
        alpha = sqrt(2 / length);
      s += vector[k] * alpha * cos(k * M_PI * ((2 * i + 1) / (2 * length)));
    }

    f[i] = s;
  }

  memcpy(f, vector, length * sizeof(double));
  free(f);

  return 0;
}

int dct2d(double* matrix, size_t width, size_t height) {
  double* temp = malloc((width > height ? width : height) * sizeof(double));
  if (!temp) return -1;

  // DCT per righe
  for (size_t row = 0; row < height; ++row) {
    memcpy(temp, &matrix[row * width], width * sizeof(double));
    dct1d(temp, width);
    memcpy(&matrix[row * width], temp, width * sizeof(double));
  }

  // DCT per colonne
  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < height; ++row)
      temp[row] = matrix[row * width + col];

    dct1d(temp, height);

    for (size_t row = 0; row < height; ++row)
      matrix[row * width + col] = temp[row];
  }

  free(temp);
  return 0;
}

int idct2d(double* matrix, size_t width, size_t height) {
  double* temp = malloc((width > height ? width : height) * sizeof(double));
  if (!temp) return -1;

  // IDCT per colonne
  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < height; ++row)
      temp[row] = matrix[row * width + col];

    idct1d(temp, height);

    for (size_t row = 0; row < height; ++row)
      matrix[row * width + col] = temp[row];
  }

  // IDCT per righe
  for (size_t row = 0; row < height; ++row) {
    memcpy(temp, &matrix[row * width], width * sizeof(double));
    idct1d(temp, width);
    memcpy(&matrix[row * width], temp, width * sizeof(double));
  }

  free(temp);
  return 0;
}
