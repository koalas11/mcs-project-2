= Introduzione

// Elabora sul ruolo della DCT nella compressione di immagini e video.
// Menzione specifiche su applicazioni come JPEG.
// Fare precisazioni su come confrontiamo l'implementazione $O(n^3)$ con quella $O(n log_2 n)$.
//

La Trasformata Discreta del Coseno (DCT) costituisce uno strumento matematico essenziale nell'ambito della compressione di dati multimediali. Tale trasformata riveste un ruolo fondamentale nei moderni algoritmi di compressione di immagini e video, consentendo di convertire informazioni spaziali in componenti frequenziali che possono essere elaborate in modo efficiente.

Nel contesto della compressione delle immagini, la DCT permette di rappresentare i dati visivi concentrando la maggior parte dell'energia del segnale in pochi coefficienti significativi. Questo principio è alla base di standard diffusi come JPEG, in cui l'immagine viene suddivisa in blocchi di 8x8 pixel sui quali viene applicata la DCT. I coefficienti risultanti vengono successivamente quantizzati, consentendo l'eliminazione delle componenti ad alta frequenza meno percepibili dall'occhio umano, ottenendo così un'elevata compressione con perdite controllate di qualità.

Analogamente, nei formati di compressione video come MPEG, H.264 e H.265, la DCT viene applicata ai frame per ridurre la ridondanza spaziale, mentre altre tecniche si occupano della ridondanza temporale tra fotogrammi successivi.

Un aspetto cruciale nell'implementazione della DCT riguarda la sua complessità computazionale. L'algoritmo classico presenta una complessità di $O(n^3)$, che risulta proibitiva per applicazioni in tempo reale o per l'elaborazione di grandi volumi di dati. Nel nostro studio, confronteremo questa implementazione con algoritmi ottimizzati che raggiungono una complessità di $O(n^2 log_2 n)$, analizzando le differenze in termini di prestazioni, precisione e applicabilità in diversi contesti operativi.
