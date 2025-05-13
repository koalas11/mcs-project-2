#import "unimib-template.typ": unimib

#show: unimib.with(
  title: "Metodi del Calcolo Scientifico - Assignment 2",
  area: [Scuola di Scienza],
  department: [Dipartimento di Informatica, Sistemi e Comunicazione],
  course: [Corso di Scienze Informatiche],
  authors: (
    "Pellegrini Damiano 886261",
    "Sanvito Marco 886493",
  ),
  bibliography: bibliography(style: "ieee", "citations.bib"),
  abstract: include("chapters/abstract.typ"),
  dark: false,
  lang: "it",
  // flipped: true
)

#set cite(form: "prose")

#set heading(numbering: none)

#include "chapters/introduction.typ"

#set heading(numbering: "1.1.")

#include "chapters/1.dct2.typ"

#set heading(numbering: none)
