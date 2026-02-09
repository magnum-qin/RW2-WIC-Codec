# RW2 WIC Codec - Panasonic RAW Format Support for Windows

A Windows Imaging Component (WIC) codec that enables native support for Panasonic RW2 (RAW) files in Windows.

## Features

✅ **System-wide support** - Once installed, RW2 files work everywhere:
- Windows File Explorer (thumbnails)
- Windows Photos app
- Microsoft Paint
- Any application using WIC (Windows Imaging Component)

✅ **High-quality RAW processing** - Uses LibRaw for professional-grade image processing:
- Camera white balance
- sRGB color space
- AHD interpolation (high quality)
- Auto brightness adjustment

✅ **Native integration** - Works like built-in image formats, no separate viewer needed

## Supported Cameras

This codec supports RW2 files from Panasonic Lumix cameras, including:
- Lumix S series (S1, S1R, S1H, S5, etc.)
- Lumix G series (GH5, GH6, G9, etc.)
- Lumix GX series
- And other Panasonic cameras that produce RW2 files

## Installation

### Prerequisites

1. **Windows 10 or Windows 11** (64-bit)
2. **Administrator privileges** for installation
3. **Visual Studio 2019 or newer** (for building from source)
4. **LibRaw library** - Install via vcpkg:
   ```batch
   vcpkg install libraw:x64-windows
   ```

### Building from Source

1. Clone or download this repository

2. Install dependencies:
   ```batch
   vcpkg install libraw:x64-windows
   ```

3. Configure with CMake:
   ```batch
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
   ```

4. Build the project:
   ```batch
   cmake --build . --config Release
   ```

### Installing the Codec

1. Navigate to the `build/Release` directory (or wherever `RW2Codec.dll` is located)

2. Copy `install.bat` to the same directory as `RW2Codec.dll`

3. **Right-click `install.bat`** and select **"Run as administrator"**

4. You should see a success message

5. The codec is now installed! You may need to restart File Explorer or your applications for changes to take effect.

### Verifying Installation

1. Open File Explorer and navigate to a folder with RW2 files
2. Switch to thumbnail view - you should see previews of your RW2 images
3. Double-click an RW2 file - it should open in Windows Photos app

## Uninstalling

1. Navigate to the directory containing `uninstall.bat` and `RW2Codec.dll`
2. **Right-click `uninstall.bat`** and select **"Run as administrator"**
3. The codec will be removed from Windows

## Testing

A test program is included to verify the codec works correctly:

```batch
TestDecoder.exe path\to\your\file.rw2
```

This will:
- Load the RW2 file using WIC
- Display image information (dimensions, DPI, etc.)
- Save a BMP copy for visual verification

## Project Structure

```
RW2Codec/
├── CMakeLists.txt              # Build configuration
├── RW2Codec.def                # DLL exports
├── README.md                   # This file
├── include/
│   ├── Common.h                # GUID definitions and common headers
│   ├── ClassFactory.h          # COM class factory
│   ├── RW2Decoder.h            # Main decoder interface
│   └── RW2FrameDecode.h        # Frame decoder interface
├── src/
│   ├── DllMain.cpp             # DLL entry point
│   ├── ClassFactory.cpp        # COM factory implementation
│   ├── RW2Decoder.cpp          # IWICBitmapDecoder implementation
│   ├── RW2FrameDecode.cpp      # IWICBitmapFrameDecode + LibRaw integration
│   ├── Registration.cpp        # Registry operations
│   └── Utils.cpp               # Utility functions
├── tests/
│   └── TestDecoder.cpp         # Test application
└── scripts/
    ├── install.bat             # Installation script
    └── uninstall.bat           # Uninstallation script
```

## Technical Details

### Architecture

```
RW2 File → WIC Codec DLL → LibRaw Processing → RGB Bitmap → Windows Apps
                ↓
           COM Registration
                ↓
        System-wide availability
```

### WIC Interfaces Implemented

- **IWICBitmapDecoder** - Main decoder interface
  - File validation and initialization
  - Frame enumeration
  - Container format identification

- **IWICBitmapFrameDecode** - Frame decoding interface
  - Pixel data extraction via LibRaw
  - Size and format information
  - RGB conversion (24bpp)

### LibRaw Processing Parameters

The codec uses the following LibRaw settings for optimal quality:
- `use_camera_wb = 1` - Camera white balance
- `output_color = 1` - sRGB color space
- `output_bps = 8` - 8-bit per channel
- `user_qual = 3` - AHD interpolation (high quality)
- `no_auto_bright = 0` - Auto brightness enabled
- `use_fuji_rotate = 1` - Auto rotation enabled

### Performance

- **Output format**: 24bpp RGB (8 bits per channel)
- **Typical load time**: 2-3 seconds for 24MP images
- **Memory usage**: ~200MB for processing a single 24MP image

## Troubleshooting

### Codec doesn't register

- Make sure you're running `install.bat` as administrator
- Check that `RW2Codec.dll` is in the same directory as `install.bat`
- Verify that all dependencies (LibRaw DLLs) are present

### RW2 files still don't show thumbnails

- Restart File Explorer (Task Manager → Restart "Windows Explorer")
- Clear thumbnail cache: Delete `%LocalAppData%\Microsoft\Windows\Explorer\thumbcache_*.db`
- Restart your computer

### Some RW2 files don't open

- LibRaw may not support all camera models/firmware versions
- Try updating to the latest LibRaw version
- Check LibRaw's supported camera list

### Performance is slow

- RAW processing is computationally intensive
- Large files (>24MP) will take longer
- Consider reducing preview quality in applications if needed

## Development

### Building for Debug

```batch
cmake --build . --config Debug
```

### Running Tests

```batch
cd build/Release
TestDecoder.exe C:\path\to\test.rw2
```

### Debugging

1. Build in Debug configuration
2. Attach Visual Studio debugger to an application using WIC (e.g., explorer.exe)
3. Load an RW2 file to trigger breakpoints

## Future Enhancements

Potential improvements for future versions:

- [ ] Extract embedded JPEG thumbnails for faster preview
- [ ] Support EXIF metadata via IWICMetadataQueryReader
- [ ] Support additional Panasonic formats (RAW, RW2L)
- [ ] Add configurable color processing options
- [ ] Support other RAW formats (CR2, NEF, ARW, etc.)
- [ ] Implement thumbnail extraction from decoder

## License

This project uses:
- **LibRaw** - Licensed under LGPL 2.1 or CDDL 1.0
- **Windows SDK** - Microsoft Software License

Please ensure compliance with LibRaw licensing when distributing.

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Test thoroughly with various RW2 files
4. Submit a pull request

## Credits

- **LibRaw** - https://www.libraw.org/
- **Windows Imaging Component (WIC)** - Microsoft

## Support

For issues or questions:
1. Check the Troubleshooting section above
2. Review LibRaw documentation for RAW format support
3. Open an issue on GitHub with sample files and error details

---

**Enjoy native RW2 support on Windows!** 📷✨
