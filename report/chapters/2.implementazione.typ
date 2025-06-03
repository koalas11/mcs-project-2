= Implementazione

// This chapter describes the implementation of the project, including the C code for the Discrete Cosine Transform (DCT), the comparison code using FFTW, and the Python GUI for image compression.

In questo capitolo viene descritta l'implementazione del progetto, includendo il codice C per la Trasformata Discreta del Coseno (DCT), il codice di confronto utilizzando la libreria FFTW e l'interfaccia grafica Python per la compressione di immagini.

== Struttura del progetto

// The project is structured into several components, each serving a specific purpose in the implementation of the Discrete Cosine Transform (DCT) and image compression.

Il progetto è strutturato in diversi componenti, ciascuno con uno scopo specifico nell'implementazione della Trasformata Discreta del Coseno e nella compressione di immagini. L'architettura modulare consente una chiara separazione tra gli algoritmi di base, le implementazioni ottimizzate e l'interfaccia utente.

La struttura principale include:

- Modulo di implementazione naive della DCT in C
- Modulo di implementazione ottimizzata basata su FFTW
- Componente di benchmark per il confronto delle prestazioni
- Interfaccia grafica in Python per la visualizzazione e l'applicazione della compressione

Tale organizzazione consente un'analisi comparativa diretta tra i diversi approcci e facilita l'estensione del progetto con ulteriori ottimizzazioni o funzionalità.

== Implementazione C della DCT

// The C implementation of the Discrete Cosine Transform (DCT) is designed to not so efficiently compute the DCT-II and DCT-III transforms. The code is structured to handle both 1D and 2D transforms.

L'implementazione in C della Trasformata Discreta del Coseno è progettata per calcolare le trasformate DCT-II e DCT-III con un approccio diretto. Pur non essendo ottimizzata per l'efficienza, questa implementazione offre una chiara rappresentazione algoritmica delle formule matematiche presentate nel capitolo precedente.

Il codice è disponibile nell'appendice del progetto e supporta sia trasformate monodimensionali che bidimensionali. @dct2.c.

Questa implementazione presenta una complessità computazionale di $O(n^2)$ per la trasformata unidimensionale e $O(n^3)$ per quella bidimensionale, come discusso nella sezione teorica. L'algoritmo per la DCT-III (inversa) segue un approccio analogo, con le opportune modifiche alla formula.

== Implementazione codice per il confronto (FFTW)

// The comparison code using FFTW is designed to benchmark the performance of the DCT implementation against the Fast Fourier Transform (FFT) provided by the FFTW library. This allows for a performance comparison between the naive DCT implementation and the optimized FFT.

Il codice di confronto utilizza la libreria FFTW (Fastest Fourier Transform in the West) per implementare una versione ottimizzata della DCT. FFTW fornisce un'implementazione altamente efficiente della Fast Fourier Transform, che può essere adattata per calcolare la DCT con complessità $O(n log_2 n)$.

L'implementazione basata su FFTW è disponibile nell'appendice del progetto, nel file @dct2_fftw.c.

Il confronto tra l'implementazione naive e quella basata su FFTW viene effettuato tramite un framework di benchmark che misura il tempo di esecuzione e l'utilizzo di memoria per diverse dimensioni di input, consentendo un'analisi dettagliata delle prestazioni.

== Implementazione Python della GUI per la compressione di immagini

// The Python GUI for image compression provides a user-friendly interface to apply the DCT and visualize the results. It allows users to select images, apply the DCT, and view the compressed images in real-time.

L'interfaccia grafica Python per la compressione di immagini offre un ambiente intuitivo per applicare la DCT e visualizzarne i risultati. Consente agli utenti di selezionare immagini, applicare la DCT e visualizzare le immagini compresse in tempo reale.

L'implementazione si basa sulle seguenti tecnologie:

- PySide6 per l'interfaccia grafica
- NumPy per la manipolazione delle matrici
- PIL (Python Imaging Library) per la gestione delle immagini
- Scipy per utilizzare la DCT ottimizzata al posto della nostra implementazione naive

Le funzionalità principali dell'interfaccia includono:

- Caricamento di immagini da file
- Visualizzazione dell'immagine originale
- Applicazione della DCT con diversi livelli di compressione
- Visualizzazione dell'immagine compressa e dei coefficienti DCT
- Calcolo e visualizzazione del rapporto di compressione e dell'errore PSNR
- Salvataggio dell'immagine compressa

L'interfaccia consente inoltre di alternare tra l'implementazione naive e quella ottimizzata, permettendo un confronto visivo immediato delle prestazioni e della qualità dei risultati.
