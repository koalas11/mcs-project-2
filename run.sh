#!/usr/bin/env bash
set -euo pipefail

# run.sh – Script di build cross-platform (Linux/macOS)

echo "Avvio della build..."

# Creazione directory di build, se non esiste
if [ ! -d build ]; then
  echo "Creazione della directory build..."
  mkdir build
fi

# Spostamento nella directory di build
cd build

# Generazione dei file di progetto con CMake
echo "Esecuzione di cmake ..."
cmake .. 

# Compilazione in modalità Release
echo "Compilazione (Release)..."
cmake --build . --config Release

# Ritorno alla directory radice
cd ..

echo "Build completata con successo!"

# Attesa di un tasto prima di uscire, se eseguito interattivamente
read -p "Premi un tasto per uscire..." -n1
echo
