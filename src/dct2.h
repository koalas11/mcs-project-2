#pragma once

#ifndef DCT2_H
#define DCT2_H

#include <stdint.h>

typedef struct dct_context dct_context;

dct_context* dct_context_alloc();
void dct_context_free(dct_context* ctx);

int dct_init(dct_context *ctx);
int dct_shutdown(dct_context *ctx);

/**
 * @brief Apply dct to the given vector
 * 
 * @param vector 
 * @param length 
 * @return int result: 0 success, -1 error.
 */
int dct1d(double* base, size_t length, size_t stride, double* out);
int idct1d(double* vector, size_t length, size_t stride, double* out);

/**
 * @brief Apply dct to the given matrix
 * 
 * @param matrix Matrix to apply dct in-place
 * @param width 
 * @param height 
 * @return int result: 0 success, -1 error.
 */
int dct2d(double* matrix, size_t width, size_t height);
int idct2d(double* matrix, size_t width, size_t height);

#endif
