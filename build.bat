@echo off
setlocal

echo RW2 WIC Codec build
echo ===================
echo.

where cmake >nul 2>&1
if not errorlevel 1 goto :cmake_ok
echo ERROR: CMake was not found in PATH.
echo Install CMake 3.15 or later, or run from a Visual Studio Developer Command Prompt.
exit /b 1

:cmake_ok

if defined VCPKG_ROOT goto :vcpkg_root_ok
echo ERROR: VCPKG_ROOT is not set.
echo Example: set VCPKG_ROOT=C:\vcpkg
exit /b 1

:vcpkg_root_ok
set "TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
if exist "%TOOLCHAIN%" goto :toolchain_ok
echo ERROR: vcpkg toolchain was not found under VCPKG_ROOT.
exit /b 1

:toolchain_ok
pushd "%~dp0"

echo [1/4] Configuring CMake and manifest dependencies...
cmake -S . -B build -A x64 -DCMAKE_TOOLCHAIN_FILE="%TOOLCHAIN%" -DBUILD_TESTING=ON
if errorlevel 1 goto :failed

echo [2/4] Building Release...
cmake --build build --config Release
if errorlevel 1 goto :failed

echo [3/4] Running smoke tests...
ctest --test-dir build -C Release --output-on-failure
if errorlevel 1 goto :failed

echo [4/4] Preparing portable package files...
copy /Y "scripts\install.bat" "build\Release\install.bat" >nul
copy /Y "scripts\uninstall.bat" "build\Release\uninstall.bat" >nul
copy /Y "LICENSE" "build\Release\LICENSE" >nul
copy /Y "THIRD_PARTY_NOTICES.md" "build\Release\THIRD_PARTY_NOTICES.md" >nul
if not exist "build\Release\licenses" mkdir "build\Release\licenses"
for %%P in (libraw lcms zlib libjpeg-turbo jasper) do if exist "vcpkg_installed\x64-windows\share\%%P\copyright" copy /Y "vcpkg_installed\x64-windows\share\%%P\copyright" "build\Release\licenses\%%P.txt" >nul
if errorlevel 1 goto :failed

echo.
echo Build completed successfully.
echo Output: %CD%\build\Release
popd
exit /b 0

:failed
echo.
echo ERROR: Build failed. See the output above and TROUBLESHOOTING.md.
popd
exit /b 1
