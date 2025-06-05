= Risultati

// Questa sezione presenta i risultati ottenuti dai benchmark eseguiti sui modelli di linguaggio. I risultati sono organizzati in tabelle e grafici per facilitare la comprensione delle prestazioni dei modelli in diverse condizioni.

In questo capitolo sono presentati i risultati ottenuti dalle implementazioni della Trasformata Discreta del Coseno (DCT). I risultati sono organizzati in due categorie principali: benchmark di performance computazionale e qualità della compressione delle immagini.

== Risultati benchmark

// I risultati dei benchmark sono presentati in forma tabellare e grafica, mostrando le prestazioni dei modelli di linguaggio in termini di velocità e accuratezza. Le tabelle includono metriche come il tempo di esecuzione, l'accuratezza delle previsioni e il numero di token elaborati al secondo. I grafici visualizzano le prestazioni dei modelli in diverse condizioni, evidenziando le differenze tra i modelli e le configurazioni testate.

I benchmark eseguiti hanno consentito di confrontare le prestazioni dell'implementazione naive della DCT con l'implementazione ottimizzata basata su FFTW. Le misurazioni hanno preso in considerazione diversi parametri:

- *Tempo di esecuzione:* Come previsto dalla teoria, l'implementazione naive ha mostrato un andamento di complessità $O(n^2)$ per array di dimensione $n$, mentre l'implementazione FFTW ha confermato la complessità teorica di $O(n log_2 n)$.

- *Scalabilità:* Con l'aumentare delle dimensioni dell'input, la differenza prestazionale tra le due implementazioni è diventata sempre più pronunciata. Per immagini di piccole dimensioni ($8 times 8$ pixel), la differenza è risultata marginale, mentre per immagini di medie e grandi dimensioni, l'implementazione FFTW ha dimostrato vantaggi significativi.

- *Prestazioni su piccole dimensioni:* Fino a blocchi di dimensione 32, la nostra implementazione risulta più veloce rispetto a FFTW, in quanto quest'ultimo deve creare un planning prima di eseguire i calcoli. Questo overhead iniziale incide in particolare su dataset di dimensioni ridotte, rendendo la nostra soluzione più efficiente in questi casi.

La differenza di prestazioni è particolarmente rilevante in scenari applicativi reali, in cui la trasformata deve essere applicata ripetutamente a numerosi blocchi di immagine. L'implementazione ottimizzata ha reso possibile l'elaborazione in tempo reale anche per immagini ad alta risoluzione.

#figure(
  caption: [Confronto tempi di calcolo tra implementazioni con DCT monodimensionale],
  {
    import "../packages.typ": cetz, cetz-plot.plot
    import "../import.typ": benches
    cetz.canvas({
      import cetz.draw: set-style, translate, scale, content

      let min = calc.floor(-3)
      let max = calc.ceil(6)
      let benches-enum = benches.enumerate()

      let make-rainbow-color(division: 1, offset: 0deg) = {
        assert(division > 0, message: "division must be positive")
        range(0, division).map(i => oklch(80%, 60%, (360deg * i / division) + offset))
      }

      let cetz-color-palette = cetz.palette.new(colors: make-rainbow-color(division: 8, offset: 200deg))
      set-style(
        legend: (fill: white.transparentize(20%), anchor: "north-west", offset: (.5em, 0)),
        axes: (
          x: (
            tick: (label: (angle: 45deg, offset: 1.25em, anchor: "east")),
            label: (offset: 1.25em),
          ),
          y: (
            label: (angle: 90deg, anchor: "east", offset: 3.5em),
            grid: (stroke: 1pt + black.transparentize(90%)),
          ),
        ),
      )
      plot.plot(
        size: (12, 6),
        plot-style: cetz-color-palette.with(stroke: true),
        mark-style: cetz-color-palette.with(stroke: true, fill: true),
        x-label: [n],
        y-label: [Tempo di calcolo \[$log_(10)(s)$\]],
        y-tick-step: none,
        y-ticks: range(min, max + 1).map(i => (i, $10^#i$)),
        y-min: min - 0.75,
        y-max: max,
        // y-tick-step: 1,
        // y-ticks: range(min, max + 1),
        // y-format: v => $10^#{ v }$,
        x-min: -1,
        x-ticks: benches-enum.map(((i, it)) => (i, it.n)),
        x-tick-step: none,
        legend: "inner-north-west",
        axis-style: "left",
        y-grid: true,
        // x-grid: true,
        {
          plot.add(
            benches-enum.map(((i, it)) => (i, calc.log(it.dct2d))),
            mark: "o",
            label: [DCT-II (Ours) $O(n^2)$],
            mark-size: .125,
            line: "spline",
          )
          plot.add(
            benches-enum.map(((i, it)) => (i, calc.log(it.idct2d))),
            mark: "o",
            label: [DCT-III (Ours) $O(n^2)$],
            mark-size: .125,
            line: "spline",
          )
          plot.add(
            benches-enum.map(((i, it)) => (i, calc.log(it.fftw_dct2d))),
            mark: "o",
            label: [DCT-II (fftw) $O(n log_2 n)$],
            mark-size: .125,
            line: "spline",
          )
          plot.add(
            benches-enum.map(((i, it)) => (i, calc.log(it.fftw_idct2d))),
            mark: "o",
            label: [DCT-III (fftw) $O(n log_2 n)$],
            mark-size: .125,
            line: "spline",
          )
        },
      )
    })
  },
)

== Risultati della compressione delle immagini

// I risultati della compressione delle immagini sono presentati sotto forma di immagini originali e compresse, delineando solamente la differenza visiva e di dimensioni tra esse. Le immagini compresse sono confrontate con le originali per mostrare l'efficacia della DCT nella riduzione delle dimensioni senza una significativa perdita di qualità visiva. Le metriche di compressione, come il rapporto di compressione e la qualità dell'immagine, sono calcolate e presentate per ogni immagine compressa.

L'applicazione della Discrete Cosine Transform (DCT) alla compressione delle immagini ha prodotto risultati significativi in termini di rapporto tra qualità e dimensione del file. Sono state analizzate diverse strategie di quantizzazione dei coefficienti DCT, osservando come queste influenzino il risultato finale.

I test di compressione hanno evidenziato quanto segue:

- *Rapporto di compressione:* Anche con un rapporto di compressione elevato, l'immagine conserva comunque una riconoscibilità sufficiente.

- *Qualità percepita:* La DCT si è dimostrata particolarmente efficace nel preservare le caratteristiche visivamente rilevanti dell'immagine, anche a livelli di compressione elevati. Ciò è dovuto alla proprietà della DCT di concentrare l'energia dell'immagine nei coefficienti a bassa frequenza.

- *Artefatti di compressione:* Ad alti livelli di compressione, sono diventati visibili artefatti tipici della compressione a blocchi, in particolare ai bordi tra blocchi adiacenti. Questo fenomeno, noto come “blocking effect”, rappresenta il principale limite qualitativo della compressione basata su DCT.

L'interfaccia grafica sviluppata ha permesso di esplorare interattivamente questi compromessi, consentendo all'utente di selezionare il livello di compressione più adatto alle proprie esigenze, bilanciando dimensione del file e qualità visiva.

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 1$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/deer.png"),
    image("../images/deer_20_1.png"),
  ),
) <figure_deer_destroyed>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 10$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/deer.png"),
    image("../images/deer_20_10.png"),
  ),
) <figure_deer_gibbs>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 3$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/80x80.png"),
    image("../images/80x80_5_1.png"),
  ),
) <figure_checkerboard_equal>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 3$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/80x80.png"),
    image("../images/80x80_20_3.png"),
  ),
) <figure_checkerboard_lo>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 274$ e $d = 20$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/bridge.png"),
    image("../images/bridge_274_20.png"),
  ),
) <figure_bridge_lo>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 274$ e $d = 30$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/bridge.png"),
    image("../images/bridge_274_30.png"),
  ),
) <figure_bridge_hi>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 200$ e $d = 20$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/cathedral.png"),
    image("../images/cathedral_200_20.png"),
  ),
) <figure_cathedral_lo>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 200$ e $d = 30$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/cathedral.png"),
    image("../images/cathedral_200_30.png"),
  ),
) <figure_cathedral_hi>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 1$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/prova.png"),
    image("../images/prova_20_1.png"),
  ),
) <figure_c_lo>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 20$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/prova.png"),
    image("../images/prova_20_20.png"),
  ),
) <figure_c_hi>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 20$ e $d = 1$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/shoe.png"),
    image("../images/shoe_20_1.png"),
  ),
) <figure_shoe_destroyed>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 50$ e $d = 10$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/bird.png"),
    image("../images/bird_50_10.png"),
  ),
) <figure_loki_lo>

#figure(
  caption: [Sinistra: immagine non compressa. Destra: immagine compressa con $F = 50$ e $d = 20$],
  grid(
    columns: (1fr,) * 2,
    column-gutter: 0.75em,
    image("../images/bird.png"),
    image("../images/bird_50_20.png"),
  ),
) <figure_loki_hi>

Data la quantità di immagini, elenchiamo i punti salienti di ciascuna. Inizialmente, evidenziamo la presenza del fenomeno di Gibbs in @figure_deer_gibbs, @figure_c_hi, @figure_bridge_lo, @figure_loki_lo, dovuto a una semplice periodicizzazione dei valori senza riflessione simmetrica sulle ordinate.

In @figure_bridge_hi, si osserva come il rumore, dovuto al basso valore ISO della fotocamera utilizzata, scompaia dopo la compressione, creando un gradiente miscelato nel cielo.

In @figure_checkerboard_equal si nota come avendo posto $F = 5$ e $d = 1$, la compressione non abbia alterato l'immagine, mantenendo i valori di ogni blocco invariati. Questo è un caso particolare perchè i blocchi sono monocromaticia e quindi i valori sono uniformi. Tuttavia, si osserva che la compressione non ha introdotto artefatti visivi, mantenendo l'immagine perfettamente riconoscibile.

In @figure_checkerboard_lo, @figure_deer_destroyed, @figure_shoe_destroyed, si nota come una compressione con eliminazione delle frequenze troppo aggressiva preservi una parvenza dell'immagine originale, uniformando il colore del blocco, similmente a un kernel convolutivo.

Infine, in @figure_deer_gibbs, @figure_bridge_hi, @figure_loki_hi, @figure_cathedral_hi, si osserva come valori di blocchi grandi ed eliminazione di frequenze meno aggressiva possano ottenere un risultato promettente, mantenendo il soggetto raffigurato. Inoltre, come dimostrato in @figure_loki_hi e @figure_loki_lo, la compressione funziona analogamente per ogni canale di colore, consentendo la compressione di immagini a colori.
