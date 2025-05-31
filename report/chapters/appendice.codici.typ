#import "../packages.typ": codly, codly-languages

#show: codly.codly-init
#codly.codly(languages: codly-languages.codly-languages, breakable: true)

#let code-counter = counter("figure")
#code-counter.step()

#let c_files = (
  ("../../src/dct2.h", "Interfaccia C comune alle implementazioni"),
  ("../../src/dct2.c", "Implementazione C dell'algoritmo naive"),
  ("../../src/dct2_fftw.c", "Implementazione C dell'algoritmo DCT-FFT"),
)

#let python_files = (
  ("../../app/lib/DCT2Handler.py", "Gestore delle operazini DCT nella GUI"),
  ("../../app/lib/DCT2Library.py", "Mappatura dell'interfaccia C in Python"),
  ("../../app/MainWindow.py", "Entrypoint Python"),
)

#let createCodeBlock(file, language, filelabel) = {
  let code = read(file)
  code = code.replace("loadMem,", "loadMem,\n")
  let fileName = file.split("/").last()
  show figure: set block(breakable: true)
  [#figure(
      caption: [#filelabel. File: #fileName],
      supplement: [Codice],
      kind: "code",
      raw(code, block: true, lang: language, tab-size: 2),
    ) #label(fileName)]
  pagebreak(weak: true)
}

= Codici

#outline(target: figure.where(kind: "code"))

== CMake e C

#createCodeBlock("../../CMakeLists.txt", "cmake", "Project configuration")

#for (file, label) in c_files {
  createCodeBlock(file, "c", label)
}

== Python

#for (file, label) in python_files {
  createCodeBlock(file, "python", label)
}
