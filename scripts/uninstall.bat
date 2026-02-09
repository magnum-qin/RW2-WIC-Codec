@echo off
echo RW2 Codec Uninstallation
echo ========================
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

REM Unregister the DLL
echo Unregistering RW2Codec.dll...
regsvr32 /u /s "%SCRIPT_DIR%RW2Codec.dll"

if %errorLevel% equ 0 (
    echo.
    echo ✓ RW2 Codec uninstalled successfully!
    echo.
    echo The codec has been removed from Windows.
    echo RW2 files will no longer be supported by the system.
) else (
    echo.
    echo ✗ Uninstallation failed!
    echo The codec may not be currently installed.
)

echo.
pause
