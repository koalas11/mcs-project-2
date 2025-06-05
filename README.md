# Assignment - Metodi del Calcolo Scientifico - Discrete cosine transform for image compression
This project explores the implementation and optimization of the Discrete Cosine Transform (DCT) for image compression. DCT is a fundamental element in modern compression algorithms such as JPEG, as it converts spatial data into frequency components that can be effectively processed and compressed.

In the project, two implementations are developed and compared: a direct algorithm with computational complexity O(n^2) and an optimized version based on the Fast Fourier Transform (FFT) with complexity O(n log2 n).

The project architecture includes a C-language implementation of the DCT-II and DCT-III transforms, a benchmark module for comparative performance analysis and a graphical interface in Python that allows you to view the effects of compression in real time.

The results highlight the significant performance advantages of optimized implementation, particularly relevant for large images, along with the effectiveness of DCT in concentrating image energy in a few significant coefficients, allowing high compression ratios with a controlled loss of quality.

Characteristic phenomena such as the blocking effect typical of the DCT applied to blocks, the Gibbs phenomenon due to a simple periodization of the domain are also analyzed and finally parallel implementation strategies for further optimizations and future processes are discussed.
