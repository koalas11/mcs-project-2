#import "unimib-template.typ": unimib

#show: unimib.with(
  title: "Metodi del Calcolo Scientifico - Assignment 2 - Trasformata del coseno discreta per compressione immagini",
  area: [Scuola di Scienza],
  department: [Dipartimento di Informatica, Sistemi e Comunicazione],
  course: [Corso di Scienze Informatiche],
  authors: (
    "Pellegrini Damiano 886261",
    "Sanvito Marco 886493",
  ),
  bibliography: bibliography(style: "ieee", "citations.bib"),
  abstract: include "chapters/abstract.typ",
  dark: false,
  lang: "it",
  // flipped: true
)

#set image(scaling: "pixelated")
#outline(target: figure.where(kind: "code"), title: [Indice codici])
#pagebreak(weak: true)

#set heading(numbering: none)

#include "chapters/introduction.typ"

#set heading(numbering: "1.1.")

#include "chapters/1.concetti-base.typ"
#include "chapters/2.implementazione.typ"
#include "chapters/3.risultati.typ"
#include "chapters/4.conclusioni.typ"

#counter(heading).update(0)
#set heading(numbering: "A.a.")

#include "chapters/appendice.codici.typ"

#set heading(numbering: none)
