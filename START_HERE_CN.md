# 🚀 快速开始 - RW2 Codec 构建

## 📌 当前状态

经过检测，您的系统需要安装以下工具：

- ❌ **Visual Studio** - 未找到
- ❌ **CMake** - 未找到
- ❌ **vcpkg** - 未找到
- ✅ **Git** - 已安装

---

## ⚡ 最快安装方式

### 选项 1: 使用自动化脚本（推荐）

我已经创建了一个自动化安装和构建脚本。

**运行步骤：**

1. **先安装 Visual Studio** (必须手动安装)
   - 下载地址: https://visualstudio.microsoft.com/zh-hans/downloads/
   - 选择 **Visual Studio 2022 Community**（免费版）
   - 安装时勾选："**使用 C++ 的桌面开发**"
   - 等待安装完成（约 30-60 分钟）

2. **运行自动化脚本**
   ```batch
   # 在项目目录下，双击运行：
   setup_and_build.bat
   ```

   这个脚本会自动：
   - ✅ 检测 Visual Studio
   - ✅ 安装 vcpkg（如果不存在）
   - ✅ 安装 LibRaw 库
   - ✅ 配置和编译项目
   - ✅ 复制所需的依赖文件

3. **完成！**
   - 构建的文件在 `build\Release\` 目录

---

### 选项 2: 手动安装（完全控制）

#### 第一步: 安装 Visual Studio 2022

1. 访问: https://visualstudio.microsoft.com/zh-hans/downloads/
2. 下载并安装 "Community" 版本
3. 在安装器中选择：
   - ✅ "使用 C++ 的桌面开发"
   - ✅ 确保包含 "CMake 工具"

#### 第二步: 安装 vcpkg

```batch
# 1. 打开命令提示符或 PowerShell
# 2. 克隆 vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg

# 3. 引导安装
cd C:\vcpkg
bootstrap-vcpkg.bat

# 4. 系统集成
vcpkg integrate install
```

#### 第三步: 安装 LibRaw

```batch
# 在 vcpkg 目录执行
C:\vcpkg\vcpkg install libraw:x64-windows
```

这一步会下载和编译 LibRaw 及其依赖，需要 **5-15 分钟**。

#### 第四步: 构建项目

```batch
# 1. 进入项目目录
cd C:\Users\qhz00\Projects\RW2Codec

# 2. 创建构建目录
mkdir build
cd build

# 3. 配置 CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -A x64

# 4. 编译
cmake --build . --config Release
```

#### 第五步: 复制依赖

```batch
# 复制 LibRaw DLL
copy C:\vcpkg\installed\x64-windows\bin\*.dll Release\

# 复制安装脚本
copy ..\scripts\*.bat Release\
```

---

## 📦 安装 Codec 到系统

构建完成后：

```batch
cd build\Release

# 右键 install.bat → "以管理员身份运行"
```

成功后，Windows 文件资源管理器和照片应用就能直接支持 RW2 文件了！

---

## 🧪 测试

```batch
# 测试解码（需要一个真实的 RW2 文件）
TestDecoder.exe C:\path\to\your\test.rw2
```

如果成功，会显示图像信息并生成一个 BMP 文件。

---

## ⏱️ 预计时间

| 步骤 | 时间 |
|-----|------|
| 下载 Visual Studio | 5-10 分钟 |
| 安装 Visual Studio | 30-60 分钟 |
| 安装 vcpkg | 2-5 分钟 |
| 安装 LibRaw | 5-15 分钟 |
| 编译项目 | 2-5 分钟 |
| **总计** | **约 1-2 小时** |

---

## ❓ 问题排查

### "cmake 不是内部或外部命令"

**解决方案：**
1. 重启命令提示符/PowerShell
2. 或使用 Visual Studio 自带的 "Developer Command Prompt"
3. 或单独安装 CMake: https://cmake.org/download/

### "找不到 LibRaw"

**解决方案：**
```batch
# 检查是否已安装
C:\vcpkg\vcpkg list | findstr libraw

# 如果没有，重新安装
C:\vcpkg\vcpkg install libraw:x64-windows
```

### "DLL 缺失"

**解决方案：**
```batch
# 从 vcpkg 复制所有 DLL
copy C:\vcpkg\installed\x64-windows\bin\*.dll build\Release\
```

### 其他问题

查看详细的故障排除指南：
- `BUILD_GUIDE_CN.md` - 完整构建指南
- `TROUBLESHOOTING.md` - 问题排查手册

---

## 📞 需要帮助？

1. 检查 `BUILD_GUIDE_CN.md` 的详细说明
2. 查看 `TROUBLESHOOTING.md` 的常见问题
3. 查看构建日志中的具体错误

---

## ✨ 推荐流程

**最简单的方式：**

```batch
1. 安装 Visual Studio 2022 Community（勾选 C++ 桌面开发）
2. 双击运行 setup_and_build.bat
3. 等待自动完成
4. 右键 build\Release\install.bat → 以管理员身份运行
5. 完成！
```

---

**准备好了吗？开始构建您的 RW2 Codec！** 🚀
