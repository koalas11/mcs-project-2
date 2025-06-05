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

2. Installa Python (versione 3.8 o superiore)
   - Scarica da [python.org](https://www.python.org/downloads/)
   - Assicurati di selezionare "Aggiungi Python al PATH" durante l'installazione

3. Installa i pacchetti Python richiesti:

   ```bat
   pip install -r requirements.txt
   ```

4. Esegui il file run.bat che compilerà le librerie e gli exe

5. Esegui l'applicazione con:

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
