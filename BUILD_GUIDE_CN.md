# RW2 WIC Codec 构建指南

本指南适用于 Windows 10/11 x64。构建过程不需要管理员权限；只有注册 Codec 时才需要管理员权限。

## 1. 安装工具

### Visual Studio 2022

安装 Visual Studio 2022，并选择：

- **使用 C++ 的桌面开发**
- MSVC v143 x64/x86 生成工具
- Windows 10 或 Windows 11 SDK
- CMake tools for Windows（推荐）

### CMake

需要 CMake 3.15 或更高版本：

```batch
cmake --version
```

### vcpkg

```batch
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
set VCPKG_ROOT=C:\vcpkg
```

如需永久设置 `VCPKG_ROOT`，请使用 Windows“环境变量”设置，而不是把个人路径提交到仓库。

## 2. 安装依赖

仓库中的 `vcpkg.json` 声明了 LibRaw：

```batch
cd D:\path\to\RW2-WIC-Codec
%VCPKG_ROOT%\vcpkg install --triplet x64-windows
```

CMake 使用 vcpkg 工具链时也会自动处理 manifest 依赖。

## 3. 配置与编译

### Visual Studio 生成器

```batch
cmake -S . -B build -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DBUILD_TESTING=ON
cmake --build build --config Debug
cmake --build build --config Release
```

### Ninja

请先从 Visual Studio Developer PowerShell 运行：

```batch
cmake -S . -B build-ninja -G Ninja ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DBUILD_TESTING=ON
cmake --build build-ninja
```

不要在同一个构建目录中混用 Visual Studio 和 Ninja 生成器。

## 4. 运行自动测试

Visual Studio 构建目录：

```batch
ctest --test-dir build -C Debug --output-on-failure
ctest --test-dir build -C Release --output-on-failure
```

Ninja 构建目录：

```batch
ctest --test-dir build-ninja --output-on-failure
```

这些是无需相机文件的启动烟雾测试。完整解码测试需要一个有权使用的 RW2 样本以及已注册的 Codec。

## 5. 准备便携测试目录

以 Release 配置为例：

```batch
mkdir package
copy build\Release\RW2Codec.dll package\
copy build\Release\Test*.exe package\
copy scripts\*.bat package\
copy vcpkg_installed\x64-windows\bin\*.dll package\
```

如果使用的是 classic vcpkg 布局，依赖 DLL 可能位于：

```text
%VCPKG_ROOT%\installed\x64-windows\bin
```

Codec 和所有依赖 DLL 必须放在同一个目录。

## 6. 注册和集成验证

在测试机器上，以管理员身份运行：

```batch
package\install.bat
```

然后执行：

```batch
package\TestDecoder.exe C:\Photos\sample.rw2
package\TestExif.exe C:\Photos\sample.rw2
```

确认资源管理器缩略图、Windows 照片、EXIF 输出和卸载流程。测试结束后，以管理员身份运行 `uninstall.bat`。

RW2 文件可能包含位置、相机序列号和作者信息，未经检查不要公开提交样本。

## 常见问题

构建失败、依赖 DLL 缺失、注册失败或缩略图不刷新时，请参阅 [TROUBLESHOOTING.md](TROUBLESHOOTING.md)。
