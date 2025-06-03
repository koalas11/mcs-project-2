= Concetti di base

== DCT

// Definizione matematica della DCT-II, DCT-III, propretà di ortogonalità.

La Trasformata Discreta del Coseno (DCT) è una trasformata matematica che converte una sequenza di dati finita in una somma di funzioni cosinusoidali con diverse frequenze. La DCT è strettamente correlata alla Trasformata di Fourier, ma utilizza esclusivamente funzioni coseno reali, risultando particolarmente efficace per la compressione dei dati.

Tra le diverse formulazioni della DCT, le più utilizzate sono la DCT-II e la sua inversa, la DCT-III. La DCT-II di una sequenza di $N$ punti $x(n)$ è definita come:

$ X(k) = sum_(n=0)^(N-1) x(n) cos [frac(pi, N) (n + frac(1, 2)) k], space.quad k = 0, 1, dots, N-1 $ <dct>

Mentre la DCT-III, che funge da inversa della DCT-II, è definita come:

$
  x(n) = frac(1, 2)X(0) + sum_(k=1)^(N-1) X(k) cos[frac(pi, N) k (n + frac(1, 2))], space.quad n = 0, 1, dots, N-1
$ <idct>

Una proprietà fondamentale della DCT è la sua ortogonalità: le funzioni base della trasformata sono ortogonali tra loro, il che implica che l'energia del segnale viene preservata durante la trasformazione. Questa proprietà garantisce che non vi sia perdita di informazioni nel processo di trasformazione e inversione.

== Algorimo DCT

// Algoritmo naive DCT, con focus sulla DCT-II e DCT-III.

L'implementazione ingenua della DCT richiede il calcolo diretto delle formule matematiche presentate. Per la DCT-II, l'algoritmo base prevede:

1. Per ogni indice di output $k$ da $0$ a $N-1$:
  - Calcolare la somma dei prodotti tra ogni elemento di input $x(n)$ e il corrispondente valore di coseno.
  - Applicare eventuali fattori di normalizzazione.

Questo approccio diretto comporta una complessità computazionale di $O(N^2)$ per una trasformata unidimensionale e di $O(N^3)$ per una trasformata bidimensionale come quella utilizzata nelle immagini. Per la DCT-III (inversa), si segue un procedimento analogo con la formula corrispondente.

Sebbene concettualmente semplice, questo approccio diventa rapidamente inefficiente per dimensioni significative di input, motivando la ricerca di algoritmi più efficienti.

== Ottimizzazione DCT

// Introduzione sul perche è importante ottimizzare la DCT.

L'ottimizzazione della DCT è fondamentale per le applicazioni pratiche, poiché la trasformata viene generalmente applicata a grandi quantità di dati, come immagini e video. Le implementazioni naive, con complessità $O(N^3)$ per trasformate bidimensionali, risultano proibitive per applicazioni in tempo reale o per dispositivi con risorse limitate.

La riduzione della complessità computazionale comporta molteplici vantaggi:

- Elaborazione più rapida, fondamentale per applicazioni in tempo reale;
- Minore consumo energetico, critico per i dispositivi mobili;
- Possibilità di elaborare immagini e video a risoluzioni più elevate;
- Riduzione dei requisiti hardware per le implementazioni.

=== Fast Fourier Transform

// Introduzione alla FFT, come viene usata per calcolare la DCT in $O(n log_2 n)$.
// Menzione di FFTW come libreria di riferimento per la FFT.

La Fast Fourier Transform (FFT) rappresenta una svolta nell'ottimizzazione della Discrete Cosine Transform (DCT). Poiché la DCT può essere espressa in termini di FFT con opportune modifiche, la complessità computazionale si riduce da $O(N^2)$ a $O(N log_2 N)$ per trasformate unidimensionali.

L'algoritmo principale sfrutta la scomposizione della DCT in termini di FFT attraverso diverse tecniche, tra cui:

- Rappresentazione della DCT come FFT di una sequenza opportunamente riordinata;
- Utilizzo di simmetrie e periodicità per ridurre il numero di calcoli necessari;
- Applicazione di algoritmi divide-and-conquer per scomporre il problema in sottoproblemi più piccoli.

FFTW (Fastest Fourier Transform in the West) rappresenta una delle librerie di riferimento più utilizzate per il calcolo efficiente della FFT. Questa libreria implementa automaticamente diversi algoritmi ottimizzati, selezionando il più efficiente in base alle dimensioni dell'input e all'architettura hardware disponibile.

=== Parallelizzazione

// Introduzione alla parallelizzazione della DCT tramite GPU o multi-threading.

La parallelizzazione rappresenta una strategia fondamentale per ottimizzare il calcolo della DCT. Data la natura intrinsecamente parallela di molte operazioni coinvolte nella trasformata, è possibile distribuire il carico computazionale su più unità di elaborazione.

L'implementazione su GPGPU (General Purpose Graphics Processing Unit) offre vantaggi significativi:

- Architettura altamente parallela con migliaia di core di elaborazione;
- Elevata larghezza di banda di memoria, cruciale per operazioni su matrici;
- Istruzioni specializzate per operazioni matematiche come seno e coseno;
- Framework come CUDA e OpenCL che facilitano lo sviluppo di codice parallelo.

In alternativa, su sistemi multicore, il multi-threading consente di:

- Suddividere l'immagine in blocchi indipendenti elaborati da thread separati;
- Sfruttare tutti i core disponibili su CPU moderne;
- Implementare strategie di bilanciamento del carico per massimizzare l'efficienza;
- Utilizzare librerie ottimizzate come OpenMP per semplificare la parallelizzazione.

== DCT a blocchi

// Introduzione alla DCT a blocchi, come viene usata nella compressione di immagini.

Nella compressione di immagini, la Discrete Cosine Transform (DCT) viene tipicamente applicata non all'intera immagine, bensì a blocchi di dimensioni ridotte. Questo approccio a blocchi presenta numerosi vantaggi sia pratici che teorici.

Lo standard JPEG, ad esempio, suddivide l'immagine in blocchi di $8 times 8$ pixel, applicando la DCT-2D separatamente a ciascun blocco. Questa strategia offre diversi benefici:

- Riduzione della complessità computazionale, limitando le dimensioni dell'input per ogni trasformata.
- Migliore localizzazione delle caratteristiche dell'immagine, consentendo una compressione adattiva.
- Possibilità di elaborazione parallela naturale, dato che i blocchi sono indipendenti.
- Minori requisiti di memoria, poiché solo un blocco alla volta deve essere mantenuto in memoria di lavoro.

Tuttavia, l'applicazione della DCT a blocchi può introdurre artefatti visibili ai bordi dei blocchi, specialmente ad alti rapporti di compressione. Questo fenomeno, noto come “blocking effect”, rappresenta una delle limitazioni principali di questo approccio e ha portato allo sviluppo di tecniche di post-elaborazione specifiche per mitigare tali artefatti.
