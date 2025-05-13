@echo off
REM filepath: d:\Progetti\mcs-project-2\convert.bat

REM Set the path to the uic executable (adjust if necessary)
set "UIC_PATH=.venv\Scripts\pyside6-uic.exe"

REM Set the source directory (where the .ui files are located)
set "SOURCE_DIR=.\ui"

REM Set the destination directory (where the generated .py files will be placed)
set "DEST_DIR=.\app\ui_generated"

REM Create the destination directory if it doesn't exist
if not exist "%DEST_DIR%" mkdir "%DEST_DIR%"

REM filepath: d:\Progetti\mcs-project-2\convert.bat
REM Loop through all .ui files in the source directory
for %%f in ("%SOURCE_DIR%\*.ui") do (
    echo Processing file: %%f

    REM Extract the filename without the extension
    setlocal EnableDelayedExpansion
    for %%n in ("%%f") do (
        set "FILENAME=%%~nf"
        echo Filename: !FILENAME!
    )

    REM Construct the output filename
    set "OUTPUT_FILE=%DEST_DIR%\ui_!FILENAME!.py"

    REM Run uic to convert the .ui file to a .py file
    echo Calling uic: "%UIC_PATH%" "%%f" -o "!OUTPUT_FILE!"
    "%UIC_PATH%" "%%f" -o "!OUTPUT_FILE!"

    endlocal

    echo Done.
    echo.
)

echo Finished processing all .ui files.
