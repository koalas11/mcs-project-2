#ifdef HAS_FFTW
#include "dct2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>
#include <math.h>

typedef struct dct_context {
  size_t max_n;
} dct_context;

static void normalize_fftw_1d(double* data, int length) {
  double scale = 2 * length;
  for (int i = 0; i < length; i++) {
    data[i] = data[i] / scale;
  }
}

static void normalize_fftw_2d(double* data, int width, int height) {
  double scale = 4 * width * height;
  for (int i = 0; i < width * height; i++) {
    data[i] = data[i] / scale;
  }
}

static void normalize_dct1d_ortho(double* data, int length) {
  for (int i = 0; i < length; i++) {
    double ai = (i == 0 ? 1.0 / sqrt(4.0 * length) : 1.0 / sqrt(2.0 * length));
    data[i] *= ai;
  }
}

static void normalize_idct1d_ortho(double* data, int length) {
  for (int i = 0; i < length; i++) {
    double ai = (i == 0 ? 1.0 / sqrt(4.0 * length) : 1.0 / sqrt(2.0 * length));
    data[i] /= ai;
  }
}

static void normalize_dct2d_ortho(double* data, int width, int height) {
  for (int i = 0; i < height; i++) {
    double ai = (i == 0 ? 1.0 / sqrt(4.0 * height) : 1.0 / sqrt(2.0 * height));
    for (int j = 0; j < width; j++) {
      double aj = (j == 0 ? 1.0 / sqrt(4.0 * width) : 1.0 / sqrt(2.0 * width));
      data[i * width + j] *= ai * aj;
    }
  }
}

static void normalize_idct2d_ortho(double* data, int width, int height) {
  for (int i = 0; i < height; i++) {
    double ai = (i == 0 ? 1.0 / sqrt(4.0 * height) : 1.0 / sqrt(2.0 * height));
    for (int j = 0; j < width; j++) {
      double aj = (j == 0 ? 1.0 / sqrt(4.0 * width) : 1.0 / sqrt(2.0 * width));
      data[i * width + j] /= ai * aj;
    }
  }
}

DCT_API dct_context* dct_context_alloc(size_t max_n) {
  dct_context* ctx = (dct_context*)malloc(sizeof(dct_context));
  if (!ctx) return NULL;
  ctx->max_n = max_n;

  return ctx;
}

DCT_API void dct_context_free(dct_context* ctx) {
  if (!ctx) return;
  free(ctx);
}

DCT_API int dct_init(dct_context* ctx) {
  return 0;
}

DCT_API int dct_shutdown(dct_context* ctx) {
  return 0;
}

DCT_API int dct1d(dct_context* ctx,
  double* in,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !in || stride < 0) return -1;

  double* tmp = fftw_alloc_real(length);

  fftw_plan plan;
  fftw_r2r_kind kind = FFTW_REDFT10;

  plan = fftw_plan_many_r2r(1, (const int*)&length, 1, in, NULL, stride, 1, tmp, NULL, stride, 1, &kind, FFTW_ESTIMATE);

  fftw_execute(plan);

  memcpy(out, tmp, length * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_cleanup();

  normalize_dct1d_ortho(out, length);

  return 0;
}

DCT_API int idct1d(dct_context* ctx,
  double* in,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !in || stride < 0) return -1;

  double* tmp = fftw_alloc_real(length);

  fftw_plan plan;
  fftw_r2r_kind kind = FFTW_REDFT01;

  normalize_idct1d_ortho(in, length);

  plan = fftw_plan_many_r2r(1, (const int*)&length, 1, in, NULL, stride, 1, tmp, NULL, stride, 1, &kind, FFTW_ESTIMATE);

  fftw_execute(plan);

  memcpy(out, tmp, length * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_cleanup();

  normalize_fftw_1d(out, length);

  return 0;
}

DCT_API int dct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {
  double* tmp = fftw_alloc_real(width * height);

  fftw_plan plan;

  plan = fftw_plan_r2r_2d(width, height, matrix, tmp, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);

  fftw_execute(plan);

  memcpy(matrix, tmp, width * height * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_cleanup();

  normalize_dct2d_ortho(matrix, width, height);

  return 0;
}

DCT_API int idct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {
  double* tmp = fftw_alloc_real(width * height);
  fftw_plan plan;

  normalize_idct2d_ortho(matrix, width, height);

  plan = fftw_plan_r2r_2d(width, height, matrix, tmp, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);

  fftw_execute(plan);

  memcpy(matrix, tmp, width * height * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_cleanup();

  normalize_fftw_2d(matrix, width, height);

  return 0;
}

DCT_API int dct2dblk(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  // Verifica che la dimensione del blocco non superi la dimensione massima supportata
  if (blk_size > ctx->max_n) return -1;

  // Calcola quanti blocchi completi possiamo fare in ogni dimensione
  size_t num_blocks_x = width / blk_size; // Solo blocchi completi in larghezza
  size_t num_blocks_y = height / blk_size; // Solo blocchi completi in altezza

  // Se non possiamo fare almeno un blocco completo, restituisci ok ma non far nulla
  if (num_blocks_x == 0 || num_blocks_y == 0) return 0;

  double* tmp = fftw_alloc_real(width * height);

  // int32_t lengths[2] = { blk_size, blk_size };
  // int32_t total_block_length[2] = { num_blocks_x * width, num_blocks_y * height };
  // fftw_r2r_kind kinds[2] = { FFTW_REDFT10, FFTW_REDFT10 };

  size_t howmany = num_blocks_x * num_blocks_y;
  double* packed = fftw_alloc_real(howmany * blk_size * blk_size);

  for (size_t by = 0; by < num_blocks_y; ++by) {
    for (size_t bx = 0; bx < num_blocks_x; ++bx) {
      size_t block_idx = by * num_blocks_x + bx;
      double* dst = packed + block_idx * blk_size * blk_size;
      double* src = mat + by * blk_size * width + bx * blk_size;
      for (size_t y = 0; y < blk_size; ++y) {
        memcpy(dst + y * blk_size,
          src + y * width,
          blk_size * sizeof(double));
      }
    }
  }


  fftw_plan plan;

  int     rank = 2;
  int32_t n[2] = { (int)blk_size, (int)blk_size };
  int32_t inembed[2] = { (int)blk_size, (int)blk_size };
  int32_t onembed[2] = { (int)blk_size, (int)blk_size };
  int     istride = num_blocks_x, ostride = num_blocks_x;
  int     idist = blk_size * blk_size;
  int     odist = blk_size * blk_size;
  fftw_r2r_kind kinds[2] = { FFTW_REDFT10, FFTW_REDFT10 };

  plan = fftw_plan_many_r2r(
    rank, n, (int)howmany,
    mat, inembed, istride, idist,
    tmp, onembed, ostride, odist,
    kinds, FFTW_ESTIMATE);
  fftw_execute(plan);

  for (size_t by = 0; by < num_blocks_y; ++by) {
    for (size_t bx = 0; bx < num_blocks_x; ++bx) {
      size_t idx = by * num_blocks_x + bx;
      double* src = packed + idx * idist;
      double* dst = mat + (by * blk_size) * width + bx * blk_size;
      for (size_t y = 0; y < blk_size; ++y) {
        memcpy(dst + y * width,
          src + y * blk_size,
          blk_size * sizeof(double));
      }
    }
  }

  memcpy(mat, tmp, width * height * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_free(packed);
  fftw_cleanup();

  normalize_dct2d_ortho(mat, width, height);

  return 0;
}

DCT_API int idct2dblk(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  // Verifica che la dimensione del blocco non superi la dimensione massima supportata
  if (blk_size > ctx->max_n) return -1;

  // Calcola quanti blocchi completi possiamo fare in ogni dimensione
  size_t num_blocks_x = width / blk_size; // Solo blocchi completi in larghezza
  size_t num_blocks_y = height / blk_size; // Solo blocchi completi in altezza

  // Se non possiamo fare almeno un blocco completo, restituisci ok ma non far nulla
  if (num_blocks_x == 0 || num_blocks_y == 0) return 0;

  double* tmp = fftw_alloc_real(width * height);

  // int32_t lengths[2] = { width, height };
  // int32_t total_block_length[2] = { num_blocks_x * width, num_blocks_y * height };
  // fftw_r2r_kind kinds[2] = { FFTW_REDFT01, FFTW_REDFT01 };

  normalize_idct2d_ortho(mat, width, height);

  size_t howmany = num_blocks_x * num_blocks_y;
  double* packed = fftw_alloc_real(howmany * blk_size * blk_size);

  for (size_t by = 0; by < num_blocks_y; ++by) {
    for (size_t bx = 0; bx < num_blocks_x; ++bx) {
      size_t block_idx = by * num_blocks_x + bx;
      double* dst = packed + block_idx * blk_size * blk_size;
      double* src = mat + by * blk_size * width + bx * blk_size;
      for (size_t y = 0; y < blk_size; ++y) {
        memcpy(dst + y * blk_size,
          src + y * width,
          blk_size * sizeof(double));
      }
    }
  }

  fftw_plan plan;

  int     rank = 2;
  int32_t n[2] = { (int)blk_size, (int)blk_size };
  int32_t inembed[2] = { (int)blk_size, (int)blk_size };
  int32_t onembed[2] = { (int)blk_size, (int)blk_size };
  int     istride = num_blocks_x, ostride = num_blocks_x;
  int     idist = blk_size * blk_size;
  int     odist = blk_size * blk_size;
  fftw_r2r_kind kinds[2] = { FFTW_REDFT10, FFTW_REDFT10 };

  plan = fftw_plan_many_r2r(
    rank, n, (int)howmany,
    mat, inembed, istride, idist,
    tmp, onembed, ostride, odist,
    kinds, FFTW_ESTIMATE);
  fftw_execute(plan);

  for (size_t by = 0; by < num_blocks_y; ++by) {
    for (size_t bx = 0; bx < num_blocks_x; ++bx) {
      size_t idx = by * num_blocks_x + bx;
      double* src = packed + idx * idist;
      double* dst = mat + (by * blk_size) * width + bx * blk_size;
      for (size_t y = 0; y < blk_size; ++y) {
        memcpy(dst + y * width,
          src + y * blk_size,
          blk_size * sizeof(double));
      }
    }
  }

  memcpy(mat, tmp, width * height * sizeof(double));

  fftw_destroy_plan(plan);
  fftw_free(tmp);
  fftw_free(packed);
  fftw_cleanup();

  normalize_fftw_2d(mat, width, height);

  return 0;
}

#endif
