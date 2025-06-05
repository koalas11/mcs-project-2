Questo progetto esplora l'implementazione e l'ottimizzazione della Trasformata Discreta del Coseno (DCT) per la compressione di immagini. La DCT rappresenta un elemento fondamentale negli algoritmi di compressione moderni come JPEG, poiché converte i dati spaziali in componenti frequenziali che possono essere efficacemente elaborati e compressi.

Nel progetto, vengono sviluppate e confrontate due implementazioni: un algoritmo diretto con complessità computazionale $O(n^2)$ e una versione ottimizzata basata sulla Fast Fourier Transform (FFT) con complessità $O(n log_2 n)$.

L'architettura del progetto comprende un'implementazione in linguaggio C delle trasformate DCT-II e DCT-III, un modulo di benchmark per l'analisi comparativa delle prestazioni e un'interfaccia grafica in Python che consente di visualizzare in tempo reale gli effetti della compressione.

I risultati evidenziano i significativi vantaggi prestazionali dell'implementazione ottimizzata, particolarmente rilevanti per immagini di grandi dimensioni, insieme all'efficacia della DCT nel concentrare l'energia dell'immagine in pochi coefficienti significativi, consentendo elevati rapporti di compressione con una perdita di qualità controllata.

Vengono inoltre analizzati fenomeni caratteristici come l'effetto di blocking tipico della DCT applicata a blocchi, il fenomeno di Gibbs dovuto a una periodicizzazione semplice del dominio e infine discusse strategie di implementazione parallela per ulteriori ottimizzazioni e lavorazioni future.
