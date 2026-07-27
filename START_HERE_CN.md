# 快速开始

## 只想安装

1. 打开[最新 Release](https://github.com/magnum-qin/RW2-WIC-Codec/releases/latest)。
2. 下载 `RW2Codec_Setup_v*.exe`。
3. 核对 GitHub 页面显示的 SHA-256 摘要。
4. 运行安装程序并允许管理员权限。
5. 如果缩略图没有刷新，重启资源管理器或 Windows。

安装程序当前没有代码签名，因此 Windows SmartScreen 可能显示警告。请只从本项目 Releases 页面下载。

## 想从源码构建

准备 Visual Studio 2022、CMake、Git 和 vcpkg，然后：

```batch
git clone https://github.com/magnum-qin/RW2-WIC-Codec.git
cd RW2-WIC-Codec
vcpkg install --triplet x64-windows
cmake -S . -B build -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

详细说明请阅读 [BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md)。

## 遇到问题

- 构建、依赖和安装问题：[TROUBLESHOOTING.md](TROUBLESHOOTING.md)
- 提交缺陷前的准备：[SUPPORT.md](SUPPORT.md)
- 参与开发：[CONTRIBUTING.md](CONTRIBUTING.md)
- 安全漏洞：[SECURITY.md](SECURITY.md)
