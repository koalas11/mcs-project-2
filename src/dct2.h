#pragma once

#ifndef DCT2_H
#define DCT2_H

struct dct_context;

int dct_init(struct dct_context *ctx);

int dct_shutdown(struct dct_context *ctx);

#endif