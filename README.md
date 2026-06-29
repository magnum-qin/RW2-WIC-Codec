# RW2 WIC Codec - Panasonic RAW Format Support for Windows

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11%20(x64)-blue.svg)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)]()

A high-performance Windows Imaging Component (WIC) codec that enables native system-wide support for Panasonic `.rw2` RAW files on Windows 10 and 11.

**[English](README.md) | [简体中文](README_CN.md) | [日本語](README_JA.md)**

---

## 📷 Features

- ✅ **Native System Integration**
  - View `.rw2` thumbnails directly in Windows File Explorer.
  - Open and zoom `.rw2` images natively in the Windows Photos app.
  - Edit RAW files directly in MS Paint, Snipping Tool, or Microsoft Office.
  - Full compatibility with any application utilizing the Windows WIC API.
- 🎨 **Professional RAW Processing**
  - Backed by the professional-grade `LibRaw` engine.
  - Respects in-camera white balance configurations.
  - Maps colors accurately to the standard sRGB color space.
  - Employs PPG interpolation (3x faster than standard AHD, while maintaining excellent image fidelity).
  - Automatically applies brightness and exposure adjustments.
- 🏷️ **Comprehensive EXIF Metadata Support**
  - Fully implements the WIC standard `IWICMetadataQueryReader` interface.
  - Native extraction of core shooting attributes: Camera Make, Model, Exposure Time, F-Number, ISO Speed, Focal Length, DateTimeOriginal, Orientation, and Image Dimensions.
  - Built-in ISO overflow protection (automatically maps ISO values larger than 65,535 to `VT_UI4` instead of `VT_UI2` to avoid truncation on modern high-ISO cameras).
- 🚀 **Performance & Memory Optimization**
  - Shared-pointer referencing model (`shared_ptr<vector<BYTE>>`) prevents redundant file buffer copies between the WIC Decoder and WIC Frame objects.
  - Fast embedded JPEG preview extraction (`GetPreview`) enables photos to open in milliseconds.
- 🔒 **Industrial-Grade Stability & Security**
  - **Dynamic Delay Loading & Isolation**: Deploys dynamic delay loading (`/DELAYLOAD:raw.dll`) combined with a thread-safe `std::call_once` loader helper. Loads all dependencies (`zlib1.dll`, `lcms2-2.dll`, `raw.dll`) locally using `LOAD_WITH_ALTERED_SEARCH_PATH`. **Never alters process-wide DLL search directories** (avoids calling global path functions like `SetDefaultDllDirectories` in `DllMain`), completely eliminating DLL hijacking and loader deadlock risks in host processes (like `explorer.exe`).
  - **COM Exception Barrier**: All COM boundary methods are wrapped in rigorous `try/catch` exception blocks. Safely converts standard library exceptions (e.g. `std::bad_alloc` to `E_OUTOFMEMORY`) and unknown errors to `E_FAIL` to protect host processes from crashes caused by corrupted RAW files.
  - **Panasonic-Specific Matching**: Custom registry pattern checks only Panasonic-specific signatures (`II` + `0x0055` / `MM` + `0x0055`), preventing registration conflicts with standard TIFF or other camera manufacturers' RAW formats (like `.arw`, `.nef`, `.cr2`).

---

## 💻 System Requirements

- **Operating System**: Windows 10 or Windows 11 (64-bit / x64 only).
- **Privileges**: Administrator privileges are required to register or unregister the COM component.
- **Development Prerequisites (Source Builds)**: Visual Studio 2022 / CMake 3.15+ / vcpkg package manager.

---

## 📥 Installation

### Method 1: Using Precompiled Releases (Recommended)

1. Download the latest release archive from the GitHub Releases page.
2. Extract the files to a directory of your choice (e.g., `C:\Program Files\RW2Codec`).
3. Right-click `install.bat` and select **"Run as administrator"**.
4. You should see a registration success prompt.
5. Restart File Explorer (or your PC) for changes to take effect.

### Method 2: Building from Source

For detailed compilation and dependencies setup, refer to [BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md).

**Quick Steps**:
1. Install LibRaw via vcpkg:
   ```batch
   vcpkg install libraw:x64-windows
   ```
2. Build the project using the automatic build script:
   ```batch
   setup_and_build.bat
   ```
3. Register the codec DLL:
   Navigate to `build\Release`, right-click `install.bat` and run as administrator.

---

## 🗑️ Uninstallation

1. Navigate to the directory containing your codec installation.
2. Right-click `uninstall.bat` and select **"Run as administrator"**.
3. The script will clean up all registry keys and association entries.

---

## 🧪 Testing

We provide dedicated testing programs to verify codec status and capabilities:

- **Core Decoder Verification**:
  ```batch
  TestDecoder.exe C:\Path\To\your_photo.rw2
  ```
  Verifies WIC loading, prints resolution/DPI, and writes a losslessly transcoded `.bmp` replica for visual verification.
  
- **EXIF Metadata Verification**:
  ```batch
  TestExif.exe C:\Path\To\your_photo.rw2
  ```
  Verifies WIC query reader path matching and prints camera parameters retrieved from the RAW file headers.

---

## 📁 Repository Structure

```
RW2Codec/
├── CMakeLists.txt              # CMake build configuration
├── RW2Codec.def                # Module definition (DLL exports)
├── README.md                   # English Readme (This file)
├── README_CN.md                # Chinese Readme
├── README_JA.md                # Japanese Readme
├── include/
│   ├── Common.h                # COM GUIDs and headers
│   ├── ClassFactory.h          # COM IClassFactory
│   ├── RW2Decoder.h            # IWICBitmapDecoder
│   ├── RW2FrameDecode.h        # IWICBitmapFrameDecode
│   └── RW2MetadataQueryReader.h# IWICMetadataQueryReader
├── src/
│   ├── DllMain.cpp             # DLL initialization
│   ├── ClassFactory.cpp        # Factory implementation
│   ├── RW2Decoder.cpp          # WIC Decoder logic
│   ├── RW2FrameDecode.cpp      # WIC Frame decoding & LibRaw processing
│   ├── RW2MetadataQueryReader.cpp # EXIF metadata parser
│   ├── Registration.cpp        # Registry COM registration
│   └── Utils.cpp               # Safe DLL loading implementation
├── tests/
│   ├── TestDecoder.cpp         # Transcoding verification test
│   └── TestPerf.cpp            # Performance profiling test
└── scripts/
    ├── install.bat             # COM registration script
    └── uninstall.bat           # COM unregistration script
```

---

## 📄 License

- This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
- **LibRaw** is licensed under LGPL 2.1 or CDDL 1.0. Please respect its licensing terms during distribution.

---

## 🙏 Credits

- **LibRaw** - https://www.libraw.org/
- **Windows Imaging Component (WIC)** - Microsoft

---

## 🌐 Quick Translations / 语言概要

### 简体中文概要
**RW2 WIC Codec** 是一款为 Windows 10/11 系统提供松下 `.rw2` RAW 格式原生支持的编解码器。安装后，您可以在资源管理器中直接预览缩略图，在系统照片应用中双击打开，以及在画图或 Office 中直接导入照片。本编解码插件深度实现了 WIC 的 `IWICMetadataQueryReader` 接口以支持读取光圈、快门、ISO 等 EXIF 元数据，并采用延迟加载隔离机制，保证宿主程序运行的安全与稳定。
- **安装**：解压发行包，右键 `install.bat` 选择 **以管理员身份运行**。
- **卸载**：右键 `uninstall.bat` 选择 **以管理员身份运行**。

### 日本語概要
**RW2 WIC Codec** は、パナソニック製カメラのRAW画像（`.rw2`）ファイルを Windows 10/11 システム全体でネイティブにサポートするWICコーデックです。インストールするだけで、エクスプローラーでのサムネイル表示、フォトアプリでのダイレクト閲覧、ペイントやOfficeでの読み込みが可能になります。WIC標準の `IWICMetadataQueryReader` インターフェースによるEXIF情報の表示や、ホストプロセスを汚染しない安全な遅延読み込み（DLLセーフロード）設計に対応しています。
- **インストール**：アーカイブを展開し、`install.bat` を右クリックして **「管理者として実行」** を選択します。
- **アンインストール**：`uninstall.bat` を右クリックして **「管理者として実行」** を選択します。
