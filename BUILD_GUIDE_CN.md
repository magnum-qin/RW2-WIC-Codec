# RW2 Codec - 构建前准备和步骤

## 当前状态检测

根据检测，您的系统需要安装以下工具才能构建项目：

### ❌ 缺少的工具

1. **CMake** - 未找到
2. **vcpkg** - 未找到（或未在标准位置）
3. **Visual Studio** - 需要检查

---

## 📋 完整安装指南

### 步骤 1: 安装 Visual Studio

1. **下载 Visual Studio 2022 Community**（免费）
   - 访问: https://visualstudio.microsoft.com/zh-hans/downloads/
   - 下载 "Community" 版本

2. **安装时选择以下工作负载：**
   - ✅ "使用C++的桌面开发"（Desktop development with C++）
   - 确保包含：
     - MSVC v143 或更新版本
     - Windows SDK
     - CMake 工具

3. **安装完成后重启**

---

### 步骤 2: 安装 vcpkg（包管理器）

#### 方法 1: 自动安装脚本

打开 **PowerShell**（管理员模式）并运行：

```powershell
# 创建 vcpkg 目录
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# 引导安装
.\bootstrap-vcpkg.bat

# 集成到系统
.\vcpkg integrate install

# 设置环境变量
[System.Environment]::SetEnvironmentVariable('VCPKG_ROOT', 'C:\vcpkg', 'User')
```

#### 方法 2: 手动安装

1. 下载 vcpkg：https://github.com/Microsoft/vcpkg
2. 解压到 `C:\vcpkg`
3. 在该目录打开命令提示符
4. 运行 `bootstrap-vcpkg.bat`
5. 运行 `vcpkg integrate install`
6. 添加环境变量 `VCPKG_ROOT=C:\vcpkg`

---

### 步骤 3: 安装 LibRaw（通过 vcpkg）

在 PowerShell 或命令提示符中运行：

```batch
cd C:\vcpkg
vcpkg install libraw:x64-windows
```

**预计安装时间：** 5-10分钟（首次会编译依赖项）

---

### 步骤 4: 验证安装

运行以下命令验证工具已安装：

```powershell
# 检查 CMake（可能需要重启终端）
cmake --version

# 检查 vcpkg
vcpkg version

# 检查 LibRaw
vcpkg list | Select-String "libraw"
```

应该看到类似输出：
```
cmake version 3.27.x
vcpkg 2024.xx.xx
libraw:x64-windows
```

---

## 🔨 构建项目

### 选项 1: 使用提供的构建脚本

1. **设置 VCPKG_ROOT 环境变量**（如果还没有）：
   ```batch
   set VCPKG_ROOT=C:\vcpkg
   ```

2. **运行构建脚本**：
   ```batch
   cd C:\Users\qhz00\Projects\RW2Codec
   build.bat
   ```

### 选项 2: 手动 CMake 构建

1. **打开 "Developer Command Prompt for VS 2022"**
   - 在开始菜单搜索 "Developer Command Prompt"
   - 或者右键开始菜单 → Visual Studio 2022 → Developer Command Prompt

2. **导航到项目目录**：
   ```batch
   cd C:\Users\qhz00\Projects\RW2Codec
   ```

3. **创建构建目录**：
   ```batch
   mkdir build
   cd build
   ```

4. **配置 CMake**：
   ```batch
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -A x64
   ```

5. **编译项目**：
   ```batch
   cmake --build . --config Release
   ```

### 选项 3: 使用 Visual Studio IDE

1. **打开 Visual Studio 2022**

2. **选择 "打开本地文件夹"**
   - 浏览到 `C:\Users\qhz00\Projects\RW2Codec`

3. **配置 CMake 设置**：
   - 打开 `CMakeSettings.json`（如果没有则创建）
   - 添加 vcpkg toolchain 路径：
   ```json
   {
     "configurations": [
       {
         "name": "x64-Release",
         "generator": "Ninja",
         "configurationType": "Release",
         "buildRoot": "${projectDir}\\build\\${name}",
         "cmakeCommandArgs": "-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
       }
     ]
   }
   ```

4. **构建**：
   - 右键 CMakeLists.txt → 生成

---

## 📦 构建输出

成功构建后，您将在以下位置找到文件：

```
RW2Codec\build\Release\
├── RW2Codec.dll          ⭐ 主要的 Codec DLL
├── RW2Codec.lib          导入库
├── TestDecoder.exe       测试程序
└── [依赖的 DLL 文件]      LibRaw.dll 等
```

---

## 🧪 测试构建

构建完成后，测试 Codec：

```batch
cd build\Release

# 测试程序（需要一个 RW2 文件）
TestDecoder.exe C:\path\to\your\test.rw2
```

---

## 📥 安装 Codec 到系统

1. **复制安装脚本**：
   ```batch
   cd build\Release
   copy ..\..\scripts\install.bat .
   copy ..\..\scripts\uninstall.bat .
   ```

2. **运行安装**（需要管理员权限）：
   - 右键 `install.bat` → "以管理员身份运行"

3. **验证安装**：
   - 打开文件资源管理器
   - 导航到包含 RW2 文件的文件夹
   - 切换到"大图标"或"超大图标"视图
   - 应该能看到 RW2 缩略图！

---

## ❓ 常见问题

### Q: "找不到 cmake"
**A:** 安装 Visual Studio 时确保选择了 "CMake 工具"，或单独安装 CMake：
- 下载：https://cmake.org/download/
- 安装后重启终端

### Q: "LibRaw 未找到"
**A:** 确保：
1. vcpkg 已安装
2. 运行了 `vcpkg install libraw:x64-windows`
3. CMake 命令包含了 toolchain 文件参数

### Q: "无法打开 wincodec.h"
**A:** 安装 Windows SDK：
- Visual Studio Installer → 修改 → 单个组件 → 搜索 "Windows SDK"

### Q: 构建成功但缺少 LibRaw.dll
**A:** 从 vcpkg 复制 DLL：
```batch
copy C:\vcpkg\installed\x64-windows\bin\*.dll build\Release\
```

---

## 🚀 快速开始（如果已有所有工具）

如果您已经安装了所有必要工具：

```batch
# 1. 设置环境变量（仅需一次）
set VCPKG_ROOT=C:\vcpkg

# 2. 构建
cd C:\Users\qhz00\Projects\RW2Codec
build.bat

# 3. 安装
cd build\Release
copy ..\..\scripts\install.bat .
# 右键 install.bat → 以管理员身份运行
```

---

## 📞 需要帮助？

查看详细的故障排除指南：`TROUBLESHOOTING.md`

或检查构建日志中的具体错误信息。

---

**祝构建顺利！** 🎉
