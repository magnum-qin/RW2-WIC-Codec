# RW2 WIC Codec - 松下RAW格式（.rw2）的Windows原生编解码器

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11%20(x64)-blue.svg)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)]()

> Windows 图像组件 (WIC) 编解码插件，让 Windows 系统（资源管理器、照片应用等）原生支持松下相机 `.rw2` 格式 RAW 文件的预览、属性读取与极速加载。

[English](README.md) | **简体中文** | [日本語](README_JA.md)

---

## 📷 简介

**RW2 WIC Codec** 是一款专为 Windows 系统开发的高性能 WIC 图像解码器。它利用底层的专业开源图像处理库 `LibRaw` 驱动，能够将松下 Lumix 相机拍摄的 `.rw2` RAW 格式图像无缝嵌入到 Windows 的系统底层中。

安装本编解码器后，您可以在不安装任何第三方图像查看软件的情况下：
- 在 **文件资源管理器** 中直接以缩略图查看 `.rw2` 文件
- 在 **Windows 照片** 应用中双击直接打开与流畅缩放
- 在 **画图 (Paint)**、**Microsoft Office** 以及任何调用系统 WIC 接口的第三方软件中直接导入与编辑 `.rw2` 照片
- 直接在文件的“属性”窗口和资源管理器“详细信息”窗格中查看拍摄时的 EXIF 元数据

---

## ✨ 主要特性

- ⚡ **深度系统集成**：一键安装，全局生效，行为与系统内置 JPEG 格式无异。
- 🎨 **专业 RAW 渲染**：基于 LibRaw 进行底层的解析与颜色映射。
  - 支持应用相机拍摄时的白平衡设置 (Camera WB)
  - 自动渲染到标准 sRGB 色彩空间，色彩呈现准确
  - 使用 PPG 补间插值算法，在保证高清画质的前提下比传统 AHD 算法提升 3 倍的渲染速度
  - 自动进行亮度矫正
- 🏷️ **EXIF 元数据完美读取 (新功能)**：深度支持 WIC 标准的 `IWICMetadataQueryReader` 接口。
  - 原生提取拍摄参数：相机品牌 (Make)、型号 (Model)、快门速度 (ExposureTime)、光圈值 (FNumber)、ISO 感光度、焦距 (FocalLength)、拍摄时间 (DateTimeOriginal)、画面旋转方向 (Orientation) 以及原始图片宽高。
  - 对超高 ISO 感光度进行了安全溢出处理（超过 65535 时采用 `VT_UI4` 存储），确保极暗环境下拍摄的 RAW 照片也能正确解析。
- 🚀 **内存与性能双向优化**：
  - 内部采用 `shared_ptr` 数据引用共享模型，避免在解码时对海量 RAW 文件数据进行多余的内存拷贝。
  - 支持极速读取内嵌的 JPEG 预览图 (`GetPreview`)，使照片查看器能够以毫秒级速度打开图片。
- 🔒 **工业级稳定性与安全性**：
  - **动态依赖延迟加载与隔离**：彻底重构了 DLL 依赖加载机制。去除了 `DllMain` 中对系统全局 DLL 搜索路径的修改（如 `SetDefaultDllDirectories`），改用局部线程安全的 `std::call_once` 和 `LOAD_WITH_ALTERED_SEARCH_PATH` 进行沙箱化按需加载，绝不污染宿主程序（如资源管理器 explorer.exe）的 DLL 搜索路径，杜绝 DLL 劫持与 DllMain 死锁隐患。
  - **COM 边界异常屏障**：在所有暴露给 WIC 系统的 COM 接口方法上配置了完整的 `try/catch` 异常防护壁垒，将标准库内存分配异常 (`std::bad_alloc`) 转换为 `E_OUTOFMEMORY`，未知异常转换为 `E_FAIL`，防止因为 RAW 图片受损或底层算法崩溃导致宿主进程崩溃。
  - **精准签名识别**：精准匹配松下专用的 TIFF 变体签名 (`II` + `0x0055`)，在反注册时完全清理相关注册表项，不会与其他 RAW 格式（如 `.arw`、`.nef`、`.cr2` 等）产生任何系统冲突。

---

## 💻 系统要求

- **操作系统**：Windows 10 或 Windows 11 (仅支持 64位/x64)
- **权限**：安装与反注册时由于需要修改注册表以注册 COM 组件，必须使用管理员权限运行脚本
- **开发依赖 (从源码构建)**：Visual Studio 2022 / CMake 3.15+ / vcpkg

---

## 📥 安装方法

### 方式一：使用预编译的安装包（推荐）

1. 在项目 GitHub Release 页面下载最新的压缩包。
2. 解压到您希望存放编解码器的文件夹（例如 `C:\Program Files\RW2Codec`）。
3. 鼠标右键点击 `install.bat`，选择 **“以管理员身份运行”**。
4. 看到成功提示后，重启 Windows 资源管理器（或重启电脑）即可生效。

### 方式二：从源码编译构建

详细编译配置指南请参考 [BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md)。

**快速构建步骤**：
```batch
# 1. 使用 vcpkg 安装 LibRaw 依赖
vcpkg install libraw:x64-windows

# 2. 运行构建脚本进行编译
setup_and_build.bat

# 3. 运行注册脚本
cd build\Release
右键点击 install.bat -> "以管理员身份运行"
```

---

## 🗑️ 卸载方法

如果您需要移除编解码插件，只需打开对应的安装目录：
1. 右键点击 `uninstall.bat`，选择 **“以管理员身份运行”**。
2. 系统会自动反注册并彻底清除关联的注册表项。

---

## 🧪 功能验证与测试

我们内置了自动化测试程序，方便您验证插件是否完美运行。

运行核心编解码测试：
```batch
TestDecoder.exe C:\路径\至\您的照片.rw2
```
该测试会检测 WIC 接口是否成功匹配，并读取分辨率、尺寸、DPI，且无损保存出一张 BMP 格式的对比大图。

运行 EXIF 元数据提取测试：
```batch
TestExif.exe C:\路径\至\您的照片.rw2
```
该测试会详细列出该 RAW 照片中提取出来的相机品牌、快门速度、光圈、ISO、焦距、拍摄时间等 EXIF 信息。

---

## 📄 开源许可协议

- 本插件主要代码采用 **MIT License** 开源。
- 依赖的开源 RAW 解码库 **LibRaw** 遵循 LGPL 2.1 或 CDDL 1.0 协议分发。

---

## 🙏 致谢

- **LibRaw** - https://www.libraw.org/
- **Windows Imaging Component (WIC)** - Microsoft
