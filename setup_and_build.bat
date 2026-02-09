@echo off
REM ============================================
REM RW2 Codec - 自动环境设置脚本
REM ============================================
echo.
echo ========================================
echo RW2 Codec - 构建环境检测和设置
echo ========================================
echo.

REM 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [警告] 某些操作可能需要管理员权限
    echo.
)

REM ============================================
REM 步骤 1: 检测 Visual Studio
REM ============================================
echo [1/5] 检测 Visual Studio...
set "VS_FOUND=0"

REM 检查 Visual Studio 2022
if exist "C:\Program Files\Microsoft Visual Studio\2022" (
    echo   ✓ 找到 Visual Studio 2022
    set "VS_FOUND=1"
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022"
)

REM 检查 Visual Studio 2019
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019" (
    echo   ✓ 找到 Visual Studio 2019
    set "VS_FOUND=1"
    set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2019"
)

if "%VS_FOUND%"=="0" (
    echo   ✗ 未找到 Visual Studio
    echo.
    echo   请先安装 Visual Studio 2019 或 2022:
    echo   https://visualstudio.microsoft.com/zh-hans/downloads/
    echo.
    echo   安装时请选择: "使用C++的桌面开发"
    echo.
    pause
    exit /b 1
)

echo.

REM ============================================
REM 步骤 2: 检测 CMake
REM ============================================
echo [2/5] 检测 CMake...
where cmake >nul 2>&1
if %errorLevel% equ 0 (
    echo   ✓ CMake 已安装
    cmake --version | findstr /C:"version"
) else (
    echo   ✗ CMake 未找到
    echo.
    echo   选项 1: 使用 Visual Studio 自带的 CMake
    echo   选项 2: 下载独立版本: https://cmake.org/download/
    echo.
    echo   我们将尝试使用 Visual Studio 自带的 CMake...
    set "CMAKE_CUSTOM=1"
)

echo.

REM ============================================
REM 步骤 3: 检测和安装 vcpkg
REM ============================================
echo [3/5] 检测 vcpkg...

REM 检查环境变量
if defined VCPKG_ROOT (
    echo   ✓ VCPKG_ROOT 已设置: %VCPKG_ROOT%
    if exist "%VCPKG_ROOT%\vcpkg.exe" (
        echo   ✓ vcpkg 可执行文件存在
    ) else (
        echo   ✗ vcpkg.exe 未找到
        set "VCPKG_ROOT="
    )
)

REM 检查常见位置
if not defined VCPKG_ROOT (
    if exist "C:\vcpkg\vcpkg.exe" (
        echo   ✓ 找到 vcpkg: C:\vcpkg
        set "VCPKG_ROOT=C:\vcpkg"
    ) else if exist "%USERPROFILE%\vcpkg\vcpkg.exe" (
        echo   ✓ 找到 vcpkg: %USERPROFILE%\vcpkg
        set "VCPKG_ROOT=%USERPROFILE%\vcpkg"
    )
)

if not defined VCPKG_ROOT (
    echo   ✗ vcpkg 未找到
    echo.
    echo   是否要自动安装 vcpkg 到 C:\vcpkg? (需要 Git)
    choice /C YN /M "选择 (Y=是, N=否)"

    if errorlevel 2 (
        echo.
        echo   请手动安装 vcpkg:
        echo   1. git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
        echo   2. cd C:\vcpkg
        echo   3. bootstrap-vcpkg.bat
        echo   4. vcpkg integrate install
        echo.
        pause
        exit /b 1
    )

    echo.
    echo   正在安装 vcpkg...

    REM 检查 git
    where git >nul 2>&1
    if %errorLevel% neq 0 (
        echo   ✗ 未找到 Git，请先安装 Git: https://git-scm.com/
        pause
        exit /b 1
    )

    REM 克隆 vcpkg
    echo   - 克隆 vcpkg 仓库...
    git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
    if %errorLevel% neq 0 (
        echo   ✗ 克隆失败
        pause
        exit /b 1
    )

    REM 引导安装
    echo   - 引导安装 vcpkg...
    cd /d C:\vcpkg
    call bootstrap-vcpkg.bat
    if %errorLevel% neq 0 (
        echo   ✗ 引导安装失败
        pause
        exit /b 1
    )

    REM 系统集成
    echo   - 集成到系统...
    vcpkg integrate install

    set "VCPKG_ROOT=C:\vcpkg"
    echo   ✓ vcpkg 安装完成
)

echo.

REM ============================================
REM 步骤 4: 安装 LibRaw
REM ============================================
echo [4/5] 检测 LibRaw...

"%VCPKG_ROOT%\vcpkg" list | findstr /C:"libraw:x64-windows" >nul 2>&1
if %errorLevel% equ 0 (
    echo   ✓ LibRaw 已安装
) else (
    echo   ✗ LibRaw 未安装
    echo.
    echo   正在安装 LibRaw (这可能需要几分钟)...
    "%VCPKG_ROOT%\vcpkg" install libraw:x64-windows

    if %errorLevel% equ 0 (
        echo   ✓ LibRaw 安装成功
    ) else (
        echo   ✗ LibRaw 安装失败
        pause
        exit /b 1
    )
)

echo.

REM ============================================
REM 步骤 5: 构建项目
REM ============================================
echo [5/5] 构建 RW2 Codec...
echo.

REM 返回项目目录
cd /d "%~dp0"

REM 创建 build 目录
if not exist build mkdir build
cd build

REM 配置 CMake
echo   正在配置 CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -A x64

if %errorLevel% neq 0 (
    echo.
    echo   ✗ CMake 配置失败
    echo.
    echo   请检查错误信息并参考 BUILD_GUIDE_CN.md
    pause
    exit /b 1
)

echo.
echo   正在编译项目 (Release 模式)...
cmake --build . --config Release

if %errorLevel% neq 0 (
    echo.
    echo   ✗ 编译失败
    echo.
    echo   请检查错误信息并参考 BUILD_GUIDE_CN.md 或 TROUBLESHOOTING.md
    pause
    exit /b 1
)

echo.
echo ========================================
echo ✓ 构建成功！
echo ========================================
echo.
echo 输出文件位置:
echo   %CD%\Release\RW2Codec.dll
echo   %CD%\Release\TestDecoder.exe
echo.
echo 下一步:
echo   1. 复制依赖的 DLL 文件:
echo      copy "%VCPKG_ROOT%\installed\x64-windows\bin\*.dll" Release\
echo.
echo   2. 复制安装脚本:
echo      copy ..\scripts\*.bat Release\
echo.
echo   3. 安装 Codec:
echo      cd Release
echo      右键 install.bat -》 以管理员身份运行
echo.
echo   4. 测试 (需要 RW2 文件):
echo      TestDecoder.exe C:\path\to\your\file.rw2
echo.
pause

REM 自动复制依赖
echo.
echo 正在复制依赖文件...
copy "%VCPKG_ROOT%\installed\x64-windows\bin\*.dll" Release\ >nul 2>&1
copy ..\scripts\*.bat Release\ >nul 2>&1

if %errorLevel% equ 0 (
    echo ✓ 依赖文件复制完成
    echo.
    echo 所有文件已准备就绪！
    echo 打开 %CD%\Release 文件夹开始使用
)

echo.
pause
