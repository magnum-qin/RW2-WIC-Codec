# Build and Installation Troubleshooting Guide

## Common Build Issues

### Issue: "LibRaw not found via CMake"

**Symptoms:**
```
CMake Error: Could not find LibRaw
```

**Solutions:**

1. **Install LibRaw via vcpkg:**
   ```batch
   vcpkg install libraw:x64-windows
   ```

2. **Set VCPKG_ROOT environment variable:**
   ```batch
   set VCPKG_ROOT=C:\path\to\your\vcpkg
   ```

3. **Specify toolchain manually in CMake:**
   ```batch
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```

---

### Issue: "Cannot open include file: 'libraw/libraw.h'"

**Symptoms:**
```
fatal error C1083: Cannot open include file: 'libraw/libraw.h'
```

**Solutions:**

1. Verify LibRaw is installed for the correct architecture:
   ```batch
   vcpkg list
   ```
   Should show: `libraw:x64-windows`

2. Clean and rebuild:
   ```batch
   rmdir /s /q build
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
   cmake --build . --config Release
   ```

---

### Issue: "LNK2019: unresolved external symbol" (LibRaw functions)

**Symptoms:**
```
error LNK2019: unresolved external symbol "class LibRaw::LibRaw"
```

**Solutions:**

1. Ensure LibRaw is linked in CMakeLists.txt (already done in this project)

2. Verify vcpkg integration:
   ```batch
   vcpkg integrate install
   ```

3. Check that you're building for the same architecture as LibRaw:
   - If LibRaw is x64, build with: `-A x64`
   - If LibRaw is x86, build with: `-A Win32`

---

### Issue: "The system cannot find the path specified" (vcpkg)

**Symptoms:**
```
ERROR: vcpkg toolchain not found
```

**Solutions:**

1. Install vcpkg:
   ```batch
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   bootstrap-vcpkg.bat
   ```

2. Set environment variable permanently:
   - Open System Properties → Environment Variables
   - Add `VCPKG_ROOT` = `C:\path\to\vcpkg`

---

## Common Installation Issues

### Issue: "The module 'RW2Codec.dll' failed to load"

**Symptoms:**
```
regsvr32 error: DllRegisterServer entry point was not found
```

**Solutions:**

1. Verify the DLL was built correctly:
   ```batch
   dumpbin /exports RW2Codec.dll
   ```
   Should show: DllRegisterServer, DllUnregisterServer, DllGetClassObject, DllCanUnloadNow

2. Check for missing dependencies:
   ```batch
   dumpbin /dependents RW2Codec.dll
   ```
   Ensure all DLLs (especially LibRaw.dll) are in the same directory or in PATH

3. Copy LibRaw DLL to the same folder:
   ```batch
   copy %VCPKG_ROOT%\installed\x64-windows\bin\*.dll .\
   ```

---

### Issue: "Access is denied" when running install.bat

**Symptoms:**
```
Access is denied
```

**Solutions:**

1. Right-click install.bat → "Run as administrator"

2. Or open elevated command prompt:
   - Press Win+X → "Command Prompt (Admin)"
   - Navigate to the directory
   - Run: `install.bat`

---

### Issue: Codec installs but thumbnails don't appear

**Symptoms:**
- regsvr32 succeeds
- RW2 files still show generic icons

**Solutions:**

1. **Restart Windows Explorer:**
   - Open Task Manager (Ctrl+Shift+Esc)
   - Find "Windows Explorer"
   - Right-click → Restart

2. **Clear thumbnail cache:**
   ```batch
   del /f /s /q %LocalAppData%\Microsoft\Windows\Explorer\thumbcache_*.db
   ```
   Then restart Explorer

3. **Verify registration:**
   - Press Win+R → `regedit`
   - Navigate to: `HKEY_CLASSES_ROOT\CLSID\{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}`
   - Should exist with InprocServer32 subkey pointing to RW2Codec.dll

4. **Check file association:**
   - Navigate to: `HKEY_CLASSES_ROOT\CLSID\{7ED96837-96F0-4812-B211-F13C24117ED3}\Instance`
   - Should have an entry for `{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}`

5. **Reboot the computer** - Sometimes necessary for WIC changes

---

### Issue: Photos app won't open RW2 files

**Symptoms:**
- Thumbnails work
- Double-clicking shows error

**Solutions:**

1. Try opening manually:
   - Right-click RW2 file
   - "Open with" → "Photos"

2. Check Event Viewer for errors:
   - Win+R → `eventvwr`
   - Windows Logs → Application
   - Look for errors from "Photos" or "WindowsCodecs"

3. Test with test program:
   ```batch
   TestDecoder.exe C:\path\to\file.rw2
   ```
   If this works but Photos doesn't, the issue is with Photos app

4. Reset Photos app:
   ```powershell
   Get-AppxPackage *photos* | Remove-AppxPackage
   Get-AppxPackage -AllUsers *photos* | Foreach {Add-AppxPackage -DisableDevelopmentMode -Register "$($_.InstallLocation)\AppXManifest.xml"}
   ```

---

## Runtime Issues

### Issue: "The application was unable to start correctly (0xc000007b)"

**Symptoms:**
Application crashes when trying to use codec

**Solutions:**

1. **Architecture mismatch** - Ensure codec is 64-bit:
   ```batch
   dumpbin /headers RW2Codec.dll | find "machine"
   ```
   Should show: `machine (x64)`

2. Rebuild with correct architecture:
   ```batch
   cmake .. -A x64
   ```

---

### Issue: Codec is slow / Photos app freezes

**Symptoms:**
- Takes very long to load RW2 files
- Application becomes unresponsive

**Solutions:**

1. **This is expected for large RAW files** (24MP+)
   - RAW processing is CPU-intensive
   - First load will be slow
   - Thumbnails may take a few seconds

2. **Check LibRaw settings** in RW2FrameDecode.cpp:
   - user_qual = 3 is highest quality but slowest
   - Try user_qual = 0 for faster (lower quality) processing

3. **Monitor CPU usage** - should spike during processing

---

### Issue: Colors look wrong

**Symptoms:**
- Images have color cast
- Different from camera LCD or other RAW processors

**Solutions:**

1. **This is expected** - Different RAW processors make different choices:
   - This codec uses camera white balance
   - sRGB color space
   - AHD demosaicing

2. Adjust LibRaw parameters in RW2FrameDecode.cpp:
   ```cpp
   // Try different white balance
   m_rawProcessor->imgdata.params.use_camera_wb = 0;  // Disable camera WB
   m_rawProcessor->imgdata.params.use_auto_wb = 1;    // Use auto WB
   ```

3. Compare with other RAW processors:
   - Different software will produce different results
   - This is normal for RAW processing

---

## Debugging Tips

### Enable WIC Debug Logging

1. Create registry key:
   ```
   HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Tracing\WIC
   ```

2. Add DWORD values:
   - EnableConsoleTracing = 1
   - EnableFileTracing = 1

3. Check: `%TEMP%\WIC*.log`

### Debug with Visual Studio

1. Build in Debug configuration:
   ```batch
   cmake --build . --config Debug
   ```

2. Attach to process:
   - Open Visual Studio
   - Debug → Attach to Process
   - Find: explorer.exe or Photos.exe
   - Load RW2 file to trigger breakpoints

3. Check Output window for debug messages

### Test with Known Good File

1. Download sample RW2 files from camera manufacturer
2. Test with multiple cameras/files
3. Compare results with LibRaw's `dcraw_emu` utility

---

## Uninstallation Issues

### Issue: "DllUnregisterServer failed"

**Solutions:**

1. The codec may not be registered - this is OK

2. Manually delete registry keys:
   - `HKEY_CLASSES_ROOT\CLSID\{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}`
   - `HKEY_CLASSES_ROOT\CLSID\{7ED96837-96F0-4812-B211-F13C24117ED3}\Instance\{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}`

---

## Getting Help

If none of these solutions work:

1. **Check versions:**
   - Windows version: `winver`
   - Visual Studio version
   - LibRaw version: Check vcpkg

2. **Collect information:**
   - Build output
   - Installation output
   - Error messages
   - Sample RW2 file (or camera model)

3. **Test with TestDecoder.exe:**
   ```batch
   TestDecoder.exe file.rw2 > test_output.txt 2>&1
   ```

4. **Check Event Viewer:**
   - Windows Logs → Application
   - Look for errors related to WIC or the codec

5. **Open an issue** with all collected information

---

## Quick Reference

### Clean Rebuild
```batch
rmdir /s /q build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -A x64
cmake --build . --config Release
```

### Reinstall Codec
```batch
cd build\Release
copy ..\..\scripts\*.bat .
regsvr32 /u /s RW2Codec.dll
regsvr32 /s RW2Codec.dll
```

### Restart Explorer
```batch
taskkill /f /im explorer.exe
start explorer.exe
```

### Clear Thumbnail Cache
```batch
del /f /s /q %LocalAppData%\Microsoft\Windows\Explorer\thumbcache_*.db
taskkill /f /im explorer.exe
start explorer.exe
```

---

**Still having issues? Check the main README.md or open an issue on GitHub.**
