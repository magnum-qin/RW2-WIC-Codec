# RW2 WIC Codec - Quick Start Guide

## What is this?

This is a Windows codec that allows your computer to natively open Panasonic RW2 (RAW) camera files, just like JPEGs. Once installed, you can:

- See thumbnails of RW2 files in File Explorer
- Open RW2 files in Windows Photos app
- Edit RW2 files in Paint and other apps

## Quick Installation

### Option 1: Download Pre-built Binary (Coming Soon)

1. Download the latest release ZIP file
2. Extract to a folder
3. Right-click `install.bat` → "Run as administrator"
4. Done! RW2 files now work system-wide

### Option 2: Build from Source

**Prerequisites:**
- Visual Studio 2019 or newer
- vcpkg package manager

**Steps:**

1. **Install LibRaw via vcpkg:**
   ```batch
   vcpkg install libraw:x64-windows
   ```

2. **Set VCPKG_ROOT environment variable:**
   ```batch
   set VCPKG_ROOT=C:\path\to\vcpkg
   ```

3. **Run build script:**
   ```batch
   build.bat
   ```

4. **Install the codec:**
   - Go to `build\Release`
   - Copy `install.bat` to this folder
   - Right-click `install.bat` → "Run as administrator"

## Testing

After installation:

1. Open File Explorer
2. Navigate to a folder with RW2 files
3. Switch to Large Icons or Extra Large Icons view
4. You should see thumbnails!
5. Double-click any RW2 file to open in Photos app

## Uninstalling

1. Right-click `uninstall.bat` → "Run as administrator"
2. Done!

## Troubleshooting

**Thumbnails not showing?**
- Restart File Explorer (Task Manager → Restart "Windows Explorer")
- Or restart your computer

**Can't install?**
- Make sure you're running as administrator
- Check that `RW2Codec.dll` is in the same folder

**Still having issues?**
- See the full README.md for detailed troubleshooting

## Supported Cameras

Works with RW2 files from:
- Panasonic Lumix S series (S1, S5, S5II, etc.)
- Panasonic Lumix G series (GH5, GH6, G9, etc.)
- Other Panasonic cameras producing RW2 files

---

**That's it! Enjoy seamless RW2 support on Windows.** 📷
