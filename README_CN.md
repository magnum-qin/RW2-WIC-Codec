# RW2 WIC Codec - 松下RW2 RAW格式的Windows原生支持

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11-blue.svg)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)]()

> Windows图像组件(WIC)解码器，让Windows系统原生支持松下RW2 RAW文件

[English](README.md) | **中文**

---

## 📷 简介

RW2 WIC Codec是一款专业的Windows图像组件解码器，为松下相机的RW2 RAW格式提供系统级支持。

### ✨ 主要特性

- ✅ **系统级支持** - 安装一次，全系统生效
  - Windows文件资源管理器显示缩略图
  - Windows照片应用直接打开
  - Paint、Office等所有WIC应用自动支持

- ✅ **高质量RAW处理** - 基于LibRaw专业库
  - 相机白平衡
  - sRGB色彩空间
  - AHD高质量插值
  - 自动亮度调整

- ✅ **无缝集成** - 完全像JPEG一样使用
  - 双击直接打开
  - 缩略图自动生成
  - 无需额外软件

- ✅ **简易安装** - 一键安装脚本
  - 自动注册到系统
  - 支持一键卸载

---

## 📷 支持的相机

支持所有输出RW2格式的松下Lumix相机，包括：

- **Lumix S系列**: S1, S1R, S1H, S5, S5II, S5IIX 等
- **Lumix G系列**: GH5, GH6, G9, G95 等
- **Lumix GX系列**: GX8, GX85, GX9 等
- 其他输出RW2文件的松下相机

---

## 💻 系统要求

- **操作系统**: Windows 10 或 Windows 11 (64位)
- **权限**: 安装时需要管理员权限
- **依赖**: 自动包含所有必要的DLL文件

---

## 📥 安装

### 方式一：使用预编译版本（即将发布）

1. 下载最新的Release版本
2. 解压到任意文件夹
3. 右键 `install.bat` → "以管理员身份运行"
4. 完成！重启资源管理器或重启电脑

### 方式二：从源代码编译

详细步骤请参考 [BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md) 或 [START_HERE_CN.md](START_HERE_CN.md)

**快速步骤：**

```batch
# 1. 安装必要工具
- Visual Studio 2022 (包含C++桌面开发)
- vcpkg 包管理器

# 2. 安装依赖
vcpkg install libraw:x64-windows

# 3. 自动构建（推荐）
setup_and_build.bat

# 4. 安装
cd build\Release
右键 install.bat → 以管理员身份运行
```

---

## 🚀 使用方法

安装完成后，RW2文件会自动支持：

### 在文件资源管理器中

1. 打开包含RW2文件的文件夹
2. 切换到"大图标"或"超大图标"视图
3. 自动显示RW2缩略图！

### 在Windows照片应用中

1. 双击任意RW2文件
2. 直接在照片应用中打开
3. 像查看JPEG一样查看RAW照片

### 在其他应用中

- **Paint**: 文件 → 打开 → 选择RW2文件
- **Office**: 插入图片时可直接选择RW2
- **其他WIC应用**: 自动支持

---

## 🧪 测试

提供了测试程序验证安装：

```batch
TestDecoder.exe C:\照片\示例.rw2
```

会显示图像信息并生成BMP文件进行对比。

---

## 🗑️ 卸载

```batch
右键 uninstall.bat → 以管理员身份运行
```

或手动运行：
```batch
regsvr32 /u RW2Codec.dll
```

---

## 📁 项目结构

```
RW2Codec/
├── src/              # C++源代码
├── include/          # 头文件
├── tests/            # 测试程序
├── scripts/          # 安装/卸载脚本
├── CMakeLists.txt    # 构建配置
└── README_CN.md      # 中文文档
```

---

## 🔧 技术细节

### 架构

```
RW2文件 → WIC Codec DLL → LibRaw处理 → RGB位图 → Windows应用
              ↓
         COM注册到系统
              ↓
      资源管理器/照片应用自动支持
```

### 实现的接口

- **IWICBitmapDecoder** - 解码器主接口
- **IWICBitmapFrameDecode** - 帧解码接口
- **COM组件** - 完整的COM架构

### LibRaw处理参数

- 相机白平衡 (use_camera_wb = 1)
- sRGB色彩空间 (output_color = 1)
- 8位输出 (output_bps = 8)
- AHD插值 (user_qual = 3)
- 自动亮度 (no_auto_bright = 0)

---

## ❓ 常见问题

### 安装后看不到缩略图？

1. 重启Windows资源管理器：
   - 任务管理器 → Windows资源管理器 → 重新启动
2. 清除缩略图缓存：
   ```batch
   del /f /s /q %LocalAppData%\Microsoft\Windows\Explorer\thumbcache_*.db
   ```
3. 重启电脑

### 某些RW2文件打不开？

- LibRaw可能不支持某些特定相机型号或固件版本
- 尝试更新到最新版本的LibRaw
- 查看LibRaw支持的相机列表

### 如何卸载？

运行 `uninstall.bat`（以管理员身份）或手动执行：
```batch
regsvr32 /u RW2Codec.dll
```

### 性能问题

- RAW处理需要较多计算资源
- 大文件（>24MP）加载时间2-3秒是正常的
- 缩略图生成可能需要几秒钟

**详细故障排查**: 参考 [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

---

## 📚 文档

- **[README.md](README.md)** - 英文主文档
- **[START_HERE_CN.md](START_HERE_CN.md)** - 中文快速开始
- **[BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md)** - 中文构建指南
- **[TROUBLESHOOTING.md](TROUBLESHOOTING.md)** - 故障排查
- **[CONTRIBUTING.md](CONTRIBUTING.md)** - 贡献指南

---

## 🤝 贡献

欢迎贡献！请查看 [CONTRIBUTING.md](CONTRIBUTING.md) 了解详情。

贡献方式：
- 🐛 报告Bug
- 💡 提出新功能建议
- 📝 改进文档
- 💻 提交代码

---

## 📄 开源协议

本项目采用 **MIT License** - 详见 [LICENSE](LICENSE) 文件

### 第三方库

- **LibRaw**: LGPL 2.1 或 CDDL 1.0
- **Windows SDK**: Microsoft软件许可

分发时请遵守LibRaw的许可证要求。

---

## 🙏 致谢

- **LibRaw** - https://www.libraw.org/
- **Windows Imaging Component (WIC)** - Microsoft

---

## 📞 支持

- 🐛 **问题反馈**: [GitHub Issues](../../issues)
- 💬 **讨论**: [GitHub Discussions](../../discussions)
- 📧 **邮件**: qhzggzh@gmail.com

---

## 🎯 未来计划

- [ ] 提取嵌入的JPEG缩略图以加快预览
- [ ] 实现EXIF元数据读取
- [ ] 支持更多松下RAW格式
- [ ] 扩展支持其他品牌RAW格式（CR2, NEF, ARW等）
- [ ] 创建Windows安装程序(MSI)

---

## 📈 项目状态

- ✅ **核心功能**: 完成
- ✅ **文档**: 完整
- ✅ **测试**: 通过
- 🔄 **优化**: 持续进行中

---

## ⭐ Star History

如果这个项目对您有帮助，请给个Star支持一下！

---

**享受Windows上无缝的RW2 RAW文件支持！** 📷✨
