#pragma once

#ifndef FFTW_DCT2_H
#define FFTW_DCT2_H

void compute_fftw_dct2d(double* in, double* out, int width, int height);

void compute_fftw_idct2d(double* in, double* out, int width, int height);

#endif // FFTW_DCT2_H
