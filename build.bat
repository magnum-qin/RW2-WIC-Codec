@echo off
echo RW2 Codec Build Script
echo =======================
echo.

REM Check if vcpkg toolchain is available
if "%VCPKG_ROOT%"=="" (
    echo WARNING: VCPKG_ROOT environment variable not set.
    echo Please set it to your vcpkg installation directory, or
    echo provide the toolchain file path manually.
    echo.
    echo Example: set VCPKG_ROOT=C:\vcpkg
    echo.
    pause
    exit /b 1
)

set TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake

if not exist "%TOOLCHAIN%" (
    echo ERROR: vcpkg toolchain not found at: %TOOLCHAIN%
    echo Please verify your VCPKG_ROOT path.
    pause
    exit /b 1
)

echo Step 1: Creating build directory...
if not exist build mkdir build
cd build

echo.
echo Step 2: Configuring with CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN%" -DCMAKE_BUILD_TYPE=Release -A x64

if %errorLevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Make sure you have installed LibRaw: vcpkg install libraw:x64-windows
    pause
    exit /b 1
)

echo.
echo Step 3: Building Release configuration...
cmake --build . --config Release

if %errorLevel% neq 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo ✓ Build completed successfully!
echo ========================================
echo.
echo Output files are in: %CD%\Release
echo.
echo Next steps:
echo 1. Copy the following files to a single directory:
echo    - RW2Codec.dll
echo    - Any LibRaw DLL dependencies
echo    - install.bat (from scripts/)
echo    - uninstall.bat (from scripts/)
echo.
echo 2. Run install.bat as administrator to register the codec
echo.

cd ..
pause
