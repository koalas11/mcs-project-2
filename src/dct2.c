#include "dct2.h"

// #include <gsl/gsl_integration.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct dct_context {
  size_t max_n;    // size of the largest transform we'll do
  double* scratch; // single buffer of length max_n
} dct_context;

// Allocate a context capable of handling up to `max_n`‑length transforms
DCT_API dct_context* dct_context_alloc(size_t max_n) {
  dct_context* ctx = (dct_context*) malloc(sizeof(dct_context));
  if (!ctx) return NULL;
  ctx->max_n = max_n;
  ctx->scratch = (double*) malloc(sizeof(double) * max_n);
  if (!ctx->scratch) {
    free(ctx);
    return NULL;
  }
  return ctx;
}

void dct_context_free(dct_context* ctx) {
  if (!ctx) return;
  free(ctx->scratch);
  free(ctx);
}

int dct_init(dct_context* ctx) {
  return 0;
}

int dct_shutdown(dct_context* ctx) {
  return 0;
}

// 1D DCT (in‑place if out == base)
DCT_API int dct1d(dct_context* ctx,
  double* func,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !func || !out || stride == 0 || length > ctx->max_n)
    return -1;

  double alpha_0 = 1.0 / sqrt((double)length);
  double alpha_k = sqrt(2.0 / (double)length);

  double* T = ctx->scratch;
  for (size_t k = 0; k < length; ++k) {
    double sum = 0.0;
    double alpha = (k == 0) ? alpha_0 : alpha_k;
    for (size_t i = 0; i < length; ++i) {
      sum += func[i * stride]
        * cos(k * M_PI * (2.0 * (double) i + 1.0) / (2.0 * (double) length));
    }
    T[k] = alpha * sum;
  }
  // copy back (allows out == func)
  for (size_t i = 0; i < length; ++i) {
    out[i * stride] = T[i];
  }
  return 0;
}

// 1D inverse DCT
DCT_API int idct1d(dct_context* ctx,
  double* coeff,
  size_t length,
  size_t stride,
  double* out) {
  if (!ctx || !coeff || !out || stride == 0 || length > ctx->max_n)
    return -1;

  double alpha_0 = 1.0 / sqrt((double)length);
  double alpha_k = sqrt(2.0 / (double)length);

  double* T = ctx->scratch;
  for (size_t i = 0; i < length; ++i) {
    double sum = 0.0;
    for (size_t k = 1; k < length; ++k) {
      sum += coeff[k * stride]
        * cos(k * M_PI * (2.0 * (double)i + 1.0) / (2.0 * (double)length));
    }
    T[i] = sum * alpha_k + coeff[0] * alpha_0;
  }
  for (size_t i = 0; i < length; ++i) {
    out[i * stride] = T[i];
  }
  return 0;
}

// 2D DCT: rows then columns, in‑place
DCT_API int dct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {
  if (!ctx || !matrix || width * height > ctx->max_n * ctx->max_n)
    return -1;

  // First pass: DCT each row (stride = 1)
  for (size_t r = 0; r < height; ++r) {
    double* row = matrix + r * width;
    if (dct1d(ctx, row, width, 1, row) < 0)
      return -1;
  }

  // Second pass: DCT each column (stride = width)
  for (size_t c = 0; c < width; ++c) {
    double* col = matrix + c;
    if (dct1d(ctx, col, height, width, col) < 0)
      return -1;
  }

  return 0;
}

// 2D inverse DCT: rows then columns, in‑place
DCT_API int idct2d(dct_context* ctx,
  double* matrix,
  size_t width,
  size_t height) {
  if (!ctx || !matrix || width * height > ctx->max_n * ctx->max_n)
    return -1;

  // Inverse DCT each row
  for (size_t r = 0; r < height; ++r) {
    double* row = matrix + r * width;
    if (idct1d(ctx, row, width, 1, row) < 0)
      return -1;
  }

  // Inverse DCT each column
  for (size_t c = 0; c < width; ++c) {
    double* col = matrix + c;
    if (idct1d(ctx, col, height, width, col) < 0)
      return -1;
  }

  return 0;
}

void _adj_block(size_t width, size_t height,
  size_t blk_size,
  size_t* out_bw, size_t* out_bh) {
  // numero di blocchi in ciascuna direzione
  size_t nx = (width + blk_size - 1) / blk_size;
  size_t ny = (height + blk_size - 1) / blk_size;
  // dimensione effettiva dei blocchi (ceil division)
  *out_bw = (width + nx - 1) / nx;
  *out_bh = (height + ny - 1) / ny;
}


DCT_API int dct2dblkrounded(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  size_t BW, BH;
  _adj_block(width, height, blk_size, &BW, &BH);

  if (BW > ctx->max_n || BH > ctx->max_n) return -1;

  // ora dividiamo in blocchi BW×BH
  for (size_t by = 0; by < height; by += BH) {
    size_t h = (by + BH <= height ? BH : height - by);
    for (size_t bx = 0; bx < width; bx += BW) {
      size_t w = (bx + BW <= width ? BW : width - bx);

      // DCT sulle righe del blocco
      for (size_t i = 0; i < h; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (dct1d(ctx, row, w, 1, row) < 0)
          return -1;
      }
      // DCT sulle colonne del blocco
      for (size_t j = 0; j < w; ++j) {
        double* col = mat + by * width + (bx + j);
        if (dct1d(ctx, col, h, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}

// iDCT 2D a blocchi “arrotondati”
DCT_API int idct2dblkrounded(dct_context* ctx,
  double* mat,
  size_t width,
  size_t height,
  size_t blk_size) {
  if (!ctx || !mat || blk_size == 0) return -1;

  size_t BW, BH;
  _adj_block(width, height, blk_size, &BW, &BH);

  if (BW > ctx->max_n || BH > ctx->max_n) return -1;

  for (size_t by = 0; by < height; by += BH) {
    size_t h = (by + BH <= height ? BH : height - by);
    for (size_t bx = 0; bx < width; bx += BW) {
      size_t w = (bx + BW <= width ? BW : width - bx);

      // iDCT sulle righe del blocco
      for (size_t i = 0; i < h; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (idct1d(ctx, row, w, 1, row) < 0)
          return -1;
      }
      // iDCT sulle colonne del blocco
      for (size_t j = 0; j < w; ++j) {
        double* col = mat + by * width + (bx + j);
        if (idct1d(ctx, col, h, width, col) < 0)
          return -1;
      }
    }
  }
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
  
  // Se non possiamo fare almeno un blocco completo, restituisci errore
  if (num_blocks_x == 0 || num_blocks_y == 0) return -1;
  
  // Elabora solo i blocchi completi
  for (size_t by = 0; by < num_blocks_y * blk_size; by += blk_size) {
    for (size_t bx = 0; bx < num_blocks_x * blk_size; bx += blk_size) {
      // DCT sulle righe del blocco
      for (size_t i = 0; i < blk_size; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (dct1d(ctx, row, blk_size, 1, row) < 0)
          return -1;
      }
      // DCT sulle colonne del blocco
      for (size_t j = 0; j < blk_size; ++j) {
        double* col = mat + by * width + (bx + j);
        if (dct1d(ctx, col, blk_size, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}

// iDCT 2D a blocchi completi (ignora blocchi parziali)
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
  
  // Se non possiamo fare almeno un blocco completo, restituisci errore
  if (num_blocks_x == 0 || num_blocks_y == 0) return -1;
  
  // Elabora solo i blocchi completi
  for (size_t by = 0; by < num_blocks_y * blk_size; by += blk_size) {
    for (size_t bx = 0; bx < num_blocks_x * blk_size; bx += blk_size) {
      // iDCT sulle righe del blocco
      for (size_t i = 0; i < blk_size; ++i) {
        double* row = mat + (by + i) * width + bx;
        if (idct1d(ctx, row, blk_size, 1, row) < 0)
          return -1;
      }
      // iDCT sulle colonne del blocco
      for (size_t j = 0; j < blk_size; ++j) {
        double* col = mat + by * width + (bx + j);
        if (idct1d(ctx, col, blk_size, width, col) < 0)
          return -1;
      }
    }
  }
  return 0;
}
