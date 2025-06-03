#pragma once

#ifndef DCT2_H
#define DCT2_H

#if defined(_MSC_VER)
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
// Microsoft 
#if defined(DLL_EXPORT)
#define DCT_API __declspec(dllexport)
#else
#define DCT_API
#endif
#elif defined(__GNUC__)
// GCC
#if defined(DLL_EXPORT)
#define DCT_API __attribute__((visibility("default")))
#else
#define DCT_API
#endif
#else
// do nothing and hope for the best?
#define DCT_API
#pragma warning Unknown dynamic link import/export semantics.
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct dct_context dct_context;

DCT_API dct_context* dct_context_alloc(size_t max_n);
DCT_API void dct_context_free(dct_context* ctx);

DCT_API int dct_init(dct_context* ctx);
DCT_API int dct_shutdown(dct_context* ctx);

DCT_API int dct1d(dct_context* ctx, double* func, size_t length, size_t stride, double* out);
DCT_API int idct1d(dct_context* ctx, double* coeff, size_t length, size_t stride, double* out);

DCT_API int dct2d(dct_context* ctx, double* matrix, size_t width, size_t height);
DCT_API int idct2d(dct_context* ctx, double* matrix, size_t width, size_t height);

DCT_API int dct2dblk(dct_context* ctx, double* matrix, size_t width, size_t height, size_t blk_size);
DCT_API int idct2dblk(dct_context* ctx, double* matrix, size_t width, size_t height, size_t blk_size);

#endif
