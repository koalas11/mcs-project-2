= Risultati

// Questa sezione presenta i risultati ottenuti dai benchmark eseguiti sui modelli di linguaggio. I risultati sono organizzati in tabelle e grafici per facilitare la comprensione delle prestazioni dei modelli in diverse condizioni.

In questo capitolo sono presentati i risultati ottenuti dalle implementazioni della Trasformata Discreta del Coseno (DCT). I risultati sono organizzati in due categorie principali: benchmark di performance computazionale e qualità della compressione delle immagini.

== Risultati benchmark

// I risultati dei benchmark sono presentati in forma tabellare e grafica, mostrando le prestazioni dei modelli di linguaggio in termini di velocità e accuratezza. Le tabelle includono metriche come il tempo di esecuzione, l'accuratezza delle previsioni e il numero di token elaborati al secondo. I grafici visualizzano le prestazioni dei modelli in diverse condizioni, evidenziando le differenze tra i modelli e le configurazioni testate.

I benchmark eseguiti hanno consentito di confrontare le prestazioni dell’implementazione naive della DCT con l’implementazione ottimizzata basata su FFTW. Le misurazioni hanno preso in considerazione diversi parametri:

- *Tempo di esecuzione:* Come previsto dalla teoria, l’implementazione naive ha mostrato un andamento di complessità $O(n^3)$ per immagini di dimensione $n times n$, mentre l’implementazione FFTW ha confermato la complessità teorica di $O(n log_2 n)$.

- *Scalabilità:* Con l’aumentare delle dimensioni dell’input, la differenza prestazionale tra le due implementazioni è diventata sempre più pronunciata. Per immagini di piccole dimensioni (8x8 pixel), la differenza è risultata marginale, mentre per immagini di medie e grandi dimensioni, l’implementazione FFTW ha dimostrato vantaggi significativi.

La differenza di prestazioni è particolarmente rilevante in scenari applicativi reali, in cui la trasformata deve essere applicata ripetutamente a numerosi blocchi di immagine. L’implementazione ottimizzata ha reso possibile l’elaborazione in tempo reale anche per immagini ad alta risoluzione.

== Risultati della compressione delle immagini

// I risultati della compressione delle immagini sono presentati sotto forma di immagini originali e compresse, delineando solamente la differenza visiva e di dimensioni tra esse. Le immagini compresse sono confrontate con le originali per mostrare l'efficacia della DCT nella riduzione delle dimensioni senza una significativa perdita di qualità visiva. Le metriche di compressione, come il rapporto di compressione e la qualità dell'immagine, sono calcolate e presentate per ogni immagine compressa.

L’applicazione della Discrete Cosine Transform (DCT) alla compressione delle immagini ha prodotto risultati significativi in termini di rapporto tra qualità e dimensione del file. Sono state analizzate diverse strategie di quantizzazione dei coefficienti DCT, osservando come queste influenzino il risultato finale.

I test di compressione hanno evidenziato quanto segue:

- *Rapporto di compressione:* Utilizzando una quantizzazione più aggressiva dei coefficienti ad alta frequenza, sono stati ottenuti rapporti di compressione fino a 20:1, mantenendo una qualità dell’immagine accettabile per la maggior parte delle applicazioni.

- *Qualità percepita:* La DCT si è dimostrata particolarmente efficace nel preservare le caratteristiche visivamente rilevanti dell’immagine, anche a livelli di compressione elevati. Ciò è dovuto alla proprietà della DCT di concentrare l’energia dell’immagine nei coefficienti a bassa frequenza.

- *Artefatti di compressione:* Ad alti livelli di compressione, sono diventati visibili artefatti tipici della compressione a blocchi, in particolare ai bordi tra blocchi adiacenti. Questo fenomeno, noto come “blocking effect”, rappresenta il principale limite qualitativo della compressione basata su DCT.

L’interfaccia grafica sviluppata ha permesso di esplorare interattivamente questi compromessi, consentendo all’utente di selezionare il livello di compressione più adatto alle proprie esigenze, bilanciando dimensione del file e qualità visiva.
