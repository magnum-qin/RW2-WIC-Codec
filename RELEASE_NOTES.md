# 🚀 Panasonic RW2 WIC Codec v1.1.2 (EXIF & Security Update)

This is a major feature and security update. Upgrading is highly recommended!
这是一个重大的功能与安全性升级版本，强烈推荐所有用户进行更新！

---

## ⚡ What's New (更新日志)

### 🏷️ Native EXIF Metadata Support (EXIF 元数据原生读取)
*   **EN**: Fully implemented the WIC-compliant `IWICMetadataQueryReader` interface. Windows File Explorer details pane and Photos app can now natively read shooting parameters (Make, Model, Exposure Time, F-Number, ISO, Focal Length, DateTimeOriginal, Orientation, and Image Dimensions). Includes high-ISO overflow safety (automatically maps values > 65535 to `VT_UI4`).
*   **ZH**: 深度支持 WIC 标准的 `IWICMetadataQueryReader` 接口。现在直接在 Windows 资源管理器的详细信息窗格或照片应用中即可看到照片拍摄参数（相机品牌、型号、曝光快门、光圈、ISO、焦距、拍摄时间、画面旋转方向及原图尺寸）。包含针对现代超高感光度相机的防溢出安全保护。

### 🔒 Dynamic Dependency Loading Isolation (无污染安全延迟加载)
*   **EN**: Completely refactored the DLL loading mechanism. Removed global DLL search modifications in `DllMain` to prevent environment pollution of host processes (like `explorer.exe`). Implemented thread-safe `std::call_once` with `LOAD_WITH_ALTERED_SEARCH_PATH` local loading for all dependencies (`raw.dll`, `lcms2-2.dll`, `zlib1.dll`), ensuring maximum security.
*   **ZH**: 彻底重构了 DLL 依赖加载机制。去除了 `DllMain` 中对系统全局 DLL 搜索路径的修改，改用局部线程安全的 `std::call_once` 和隔离目录加载，防止污染资源管理器等系统宿主进程，彻底杜绝 DLL 劫持与 DllMain 死锁隐患。

### 🖼️ Embedded JPEG Preview Extraction (内嵌预览极速提取)
*   **EN**: Added native WIC `GetPreview` support via LibRaw's embedded thumbnail unpacker. Applications can now retrieve and display the embedded JPEG preview image in milliseconds, making the photo viewing experience buttery smooth.
*   **ZH**: 实现了 WIC 的 `GetPreview` 接口。当您在照片查看器中快速翻页时，插件会以毫秒级速度直接提取并输出 RAW 文件中内嵌的 JPEG 预览图，无需完整现像，大大加快了看图预览速度。

### 🛡️ Robust COM Exception Barriers (COM 边界异常防护)
*   **EN**: Implemented rigorous `try/catch` exception barriers across all COM boundary methods. Safely converts standard library errors (such as `std::bad_alloc` to `E_OUTOFMEMORY`) to prevent host process crashes when opening corrupted RAW images.
*   **ZH**: 在所有暴露给系统的 COM 接口方法上配置了完整的异常防护屏障，将标准库内存分配等异常安全转换为对应的 COM 错误代码，防止因为 RAW 照片文件受损而导致调用编解码器的宿主进程（如资源管理器）崩溃。

### 🤖 Automated Release Pipeline (自动化构建与发布)
*   **EN**: Introduced a fully automated CI/CD pipeline using GitHub Actions. Releases are now automatically compiled, packaged into Inno Setup `.exe` installers and `.zip` archives, and published directly to GitHub.
*   **ZH**: 引入了基于 GitHub Actions 的自动化 CI/CD 构建流水线。每次发布版本时，云端自动编译、使用 Inno Setup 打包 `.exe` 安装程序和 `.zip` 压缩包，并自动发布到 GitHub Release。

---

## 📦 Package Assets (包含的资产)

*   `RW2Codec_Setup_v1.1.2.exe`: The recommended modern installer. Just double-click to install.
*   `RW2Codec-v1.1.2-x64.zip`: Portable zip containing raw DLLs and command-line installation scripts (`install.bat` / `uninstall.bat`).
