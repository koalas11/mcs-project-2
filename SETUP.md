# Guida all'Installazione

Questo documento fornisce istruzioni per la compilazione e l'esecuzione del progetto.

## Report

Per generare il report, è necessario:

1. Scaricare l'estensione TinyMist per VS Code
2. Compilare il file main.typ utilizzando l'estensione

## Requisiti di Sistema

### Windows

1. Installa Visual Studio 2022 con gli strumenti per la compilazione C/C++
   - Durante l'installazione, seleziona "Sviluppo di applicazioni desktop con C++"

2. Installare i binari precompilati di FFTW nella cartella lib situata nella root del progetto. Successivamente, creare due file denominati FFTW3Config.cmake e FFTW3ConfigVersion.cmake, inserendoli nella stessa cartella lib con i seguenti contenuti

   FFTW3ConfigVersion.cmake
   ```cmake
   # defined since 2.8.3
   if (CMAKE_VERSION VERSION_LESS 2.8.3)
   get_filename_component (CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
   endif ()

   # Allows loading FFTW3 settings from another project
   set (FFTW3_CONFIG_FILE "${CMAKE_CURRENT_LIST_FILE}")

   set (FFTW3_LIBRARIES libfftw3-3)
   set (FFTW3_LIBRARY_DIRS ${CMAKE_CURRENT_LIST_DIR})
   set (FFTW3_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})

   include ("${CMAKE_CURRENT_LIST_DIR}/FFTW3LibraryDepends.cmake" OPTIONAL)

   if (CMAKE_VERSION VERSION_LESS 2.8.3)
   set (CMAKE_CURRENT_LIST_DIR)
   endif ()
   ```

   FFTW3Config.cmake
   ```cmake
   set (PACKAGE_VERSION "3.3.10")
   # Check whether the requested PACKAGE_FIND_VERSION is compatible
   if ("${PACKAGE_VERSION}" VERSION_LESS "${PACKAGE_FIND_VERSION}")
   set (PACKAGE_VERSION_COMPATIBLE FALSE)
   else ()
   set (PACKAGE_VERSION_COMPATIBLE TRUE)
   if ("${PACKAGE_VERSION}" VERSION_EQUAL "${PACKAGE_FIND_VERSION}")
      set (PACKAGE_VERSION_EXACT TRUE)
   endif ()
   endif ()
   ```

3. Installa Python (versione 3.8 o superiore)
   - Scarica da [python.org](https://www.python.org/downloads/)
   - Assicurati di selezionare "Aggiungi Python al PATH" durante l'installazione

4. Installa i pacchetti Python richiesti:

   ```bat
   pip install -r requirements.txt
   ```

5. Esegui il file run.bat che compilerà le librerie e gli exe

6. Esegui l'applicazione con:

   ```bat
   python main.py
   ```

### Linux

1. Installa i pacchetti necessari per la compilazione C:

   ```sh
   sudo apt-get update
   sudo apt-get install build-essential cmake make gcc libfftw3
   ```

2. Installa Python e pip:

   ```sh
   sudo apt-get install python3 python3-pip
   ```

3. Installa i pacchetti Python richiesti:

   ```sh
   pip3 install -r requirements.txt
   ```

4. Esegui il file di compilazione:

   ```sh
   bash run.sh
   ```

5. Esegui l'applicazione con:

   ```sh
   python3 main.py
   ```

### macOS

1. Installa i pacchetti necessari per la compilazione C:

   ```sh
   brew install cmake fftw
   ```

   (Richiede [Homebrew](https://brew.sh/))

2. Installa Python (se non già presente):

   ```sh
   brew install python
   ```

3. Installa i pacchetti Python richiesti:

   ```sh
   pip3 install -r requirements.txt
   ```

4. Esegui il file di compilazione:

   ```sh
   bash run.sh
   ```

5. Esegui l'applicazione con:

   ```sh
   python3 main.py
   ```
