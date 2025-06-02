#let benches = csv("../bench.csv", row-type: dictionary).map(it => (
  n: int(it.n),
  dct2d: float(it.dct2d_s),
  idct2d: float(it.idct2d_s),
  fftw_dct2d: float(it.fftw_dct2d_s),
  fftw_idct2d: float(it.fftw_idct2d_s),
))
