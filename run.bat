@echo off
REM filepath: run.bat

REM Set Visual Studio environment variables
echo Setting Visual Studio environment variables...
if not defined VSCMD_VER (
    echo Calling vcvars64.bat to set environment variables...
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    if %ERRORLEVEL% NEQ 0 (
        echo Failed to set Visual Studio environment variables.
        exit /b errorcode
    )
) else (
    echo Visual Studio environment variables are already set. Skipping vcvars64.bat.
)

REM Navigate to build_windows directory and build the project
echo Building project in build_windows directory...
if not exist build_windows mkdir build_windows
cd build_windows
cmake ..
if %ERRORLEVEL% NEQ 0 (
    cd ..
    exit /b errorcode
)

cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    cd ..
    echo Build completely or partially failed
    exit /b errorcode
)

REM Navigate back to root directory
cd ..

echo Done!

pause
