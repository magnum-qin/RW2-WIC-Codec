@echo off
setlocal

echo RW2 WIC Codec environment check
echo ===============================
echo.

where git >nul 2>&1
if not errorlevel 1 goto :git_ok
echo ERROR: Git was not found in PATH.
exit /b 1

:git_ok

where cmake >nul 2>&1
if not errorlevel 1 goto :cmake_ok
echo ERROR: CMake was not found in PATH.
echo Install Visual Studio 2022 with C++ CMake tools or install CMake separately.
exit /b 1

:cmake_ok
if defined VCPKG_ROOT goto :vcpkg_root_ok
if exist "C:\vcpkg\vcpkg.exe" set "VCPKG_ROOT=C:\vcpkg"
if defined VCPKG_ROOT goto :vcpkg_root_ok
echo ERROR: VCPKG_ROOT is not set and C:\vcpkg was not found.
echo.
echo Install vcpkg:
echo   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
echo   C:\vcpkg\bootstrap-vcpkg.bat
echo   set VCPKG_ROOT=C:\vcpkg
exit /b 1

:vcpkg_root_ok
if exist "%VCPKG_ROOT%\vcpkg.exe" goto :vcpkg_exe_ok
echo ERROR: vcpkg.exe was not found under VCPKG_ROOT.
exit /b 1

:vcpkg_exe_ok
echo Git:
git --version
echo.
echo CMake:
cmake --version
echo.
echo vcpkg:
"%VCPKG_ROOT%\vcpkg.exe" version
echo.

echo Environment check passed. Starting the reproducible manifest build...
call "%~dp0build.bat"
exit /b %errorlevel%
