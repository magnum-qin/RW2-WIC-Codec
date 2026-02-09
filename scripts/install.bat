@echo off
echo RW2 Codec Installation
echo =======================
echo.

REM Check for admin rights
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERROR: This script requires administrator privileges.
    echo Please right-click and select "Run as administrator"
    pause
    exit /b 1
)

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0

REM Register the DLL
echo Registering RW2Codec.dll...
regsvr32 /s "%SCRIPT_DIR%RW2Codec.dll"

if %errorLevel% equ 0 (
    echo.
    echo ✓ RW2 Codec installed successfully!
    echo.
    echo The codec is now registered with Windows.
    echo You should now be able to view RW2 files in:
    echo   - File Explorer (thumbnails)
    echo   - Windows Photos app
    echo   - Paint and other WIC-compatible applications
    echo.
    echo Note: You may need to restart Explorer or your applications
    echo       for the changes to take effect.
) else (
    echo.
    echo ✗ Installation failed!
    echo Please make sure RW2Codec.dll is in the same directory as this script.
)

echo.
pause
