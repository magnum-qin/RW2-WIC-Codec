# Changelog

All notable changes to the RW2 WIC Codec project will be documented in this file.

## [1.0.0] - 2026-02-10

### Added
- Initial release of RW2 WIC Codec
- Support for Panasonic RW2 RAW format
- WIC decoder implementation (IWICBitmapDecoder)
- Frame decoder implementation (IWICBitmapFrameDecode)
- LibRaw integration for RAW processing
- 24bpp RGB output format
- COM registration and unregistration
- Installation and uninstallation scripts
- Test program for verification
- Comprehensive documentation

### Features
- System-wide RW2 support in Windows
- Thumbnail display in File Explorer
- Integration with Windows Photos app
- Support for all WIC-compatible applications
- High-quality AHD interpolation
- Camera white balance processing
- Auto brightness adjustment
- Auto rotation support

### Technical
- Implements IWICBitmapDecoder interface
- Implements IWICBitmapFrameDecode interface
- Thread-safe COM implementation
- Proper reference counting
- Memory-efficient stream processing
- Error handling with proper HRESULT codes

### Known Limitations
- No embedded thumbnail extraction (processes full RAW)
- No EXIF metadata exposure via WIC
- Single frame support only (appropriate for RAW)
- Requires LibRaw runtime dependency

## [Unreleased]

### Planned Features
- Embedded JPEG thumbnail extraction for faster previews
- EXIF metadata reader implementation
- Support for additional Panasonic formats (RAW, RW2L)
- Configurable color processing options
- Multi-format RAW codec (CR2, NEF, ARW support)
- Performance optimizations
- Installer package (MSI/EXE)

---

Format based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)
