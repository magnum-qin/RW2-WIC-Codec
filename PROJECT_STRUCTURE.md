# RW2 WIC Codec - Project Structure

```
RW2Codec/
│
├── 📋 Documentation Files
│   ├── README.md                      # Main documentation
│   ├── QUICKSTART.md                  # Quick start guide
│   ├── IMPLEMENTATION_SUMMARY.md      # Implementation details
│   ├── TROUBLESHOOTING.md             # Troubleshooting guide
│   ├── CHANGELOG.md                   # Version history
│   └── CONTRIBUTING.md                # Contribution guidelines
│
├── 🔧 Build Configuration
│   ├── CMakeLists.txt                 # CMake build configuration
│   ├── RW2Codec.def                   # DLL export definitions
│   ├── build.bat                      # Automated build script
│   └── .gitignore                     # Git ignore patterns
│
├── 📁 include/                        # Header files
│   ├── Common.h                       # GUID definitions, macros, common includes
│   ├── ClassFactory.h                 # COM class factory interface
│   ├── RW2Decoder.h                   # IWICBitmapDecoder interface
│   └── RW2FrameDecode.h               # IWICBitmapFrameDecode interface
│
├── 📁 src/                            # Source files
│   ├── DllMain.cpp                    # DLL entry point, COM exports
│   │                                  #   - DllGetClassObject
│   │                                  #   - DllCanUnloadNow
│   │                                  #   - DllRegisterServer
│   │                                  #   - DllUnregisterServer
│   │
│   ├── ClassFactory.cpp               # COM class factory implementation
│   │                                  #   - CreateInstance
│   │                                  #   - LockServer
│   │
│   ├── RW2Decoder.cpp                 # WIC decoder implementation
│   │                                  #   - Initialize (load RW2 file)
│   │                                  #   - QueryCapability (validate file)
│   │                                  #   - GetFrame (return frame decoder)
│   │                                  #   - GetFrameCount (returns 1)
│   │
│   ├── RW2FrameDecode.cpp             # Frame decoder + LibRaw integration
│   │                                  #   - ProcessRawData (LibRaw processing)
│   │                                  #   - GetSize (image dimensions)
│   │                                  #   - CopyPixels (RGB data extraction)
│   │                                  #   - GetPixelFormat (24bpp RGB)
│   │
│   ├── Registration.cpp               # Registry operations
│   │                                  #   - RegisterCodec (system registration)
│   │                                  #   - UnregisterCodec (cleanup)
│   │
│   └── Utils.cpp                      # Utility functions
│
├── 📁 scripts/                        # Installation scripts
│   ├── install.bat                    # Install codec (run as admin)
│   └── uninstall.bat                  # Uninstall codec (run as admin)
│
└── 📁 tests/                          # Test programs
    └── TestDecoder.cpp                # WIC codec test application
                                       #   - Load RW2 via WIC
                                       #   - Display image info
                                       #   - Export to BMP

📦 Build Output (after building):
build/
├── CMakeFiles/                        # CMake generated files
├── Release/
│   ├── RW2Codec.dll                   # ⭐ Main codec DLL
│   ├── RW2Codec.lib                   # Import library
│   ├── TestDecoder.exe                # Test program
│   └── [LibRaw dependencies]          # LibRaw.dll, etc.
└── Debug/
    └── [Debug builds]
```

## File Count Summary

| Category | Count | Files |
|----------|-------|-------|
| **Source Files (.cpp)** | 7 | DllMain, ClassFactory, RW2Decoder, RW2FrameDecode, Registration, Utils, TestDecoder |
| **Header Files (.h)** | 4 | Common, ClassFactory, RW2Decoder, RW2FrameDecode |
| **Documentation (.md)** | 6 | README, QUICKSTART, IMPLEMENTATION_SUMMARY, TROUBLESHOOTING, CHANGELOG, CONTRIBUTING |
| **Build Files** | 3 | CMakeLists.txt, RW2Codec.def, .gitignore |
| **Scripts (.bat)** | 3 | build, install, uninstall |
| **Total** | **23** | |

## Code Statistics

| Metric | Count |
|--------|-------|
| Total Lines of Code (approx.) | ~2,500 |
| Source Files LOC | ~1,800 |
| Header Files LOC | ~300 |
| Test Code LOC | ~200 |
| Documentation Lines | ~1,500+ |

## Key Components

### 🔷 COM Infrastructure
- **DllMain.cpp** - Entry point, reference counting, server locks
- **ClassFactory.cpp** - Object creation and lifetime management

### 🔷 WIC Interfaces
- **RW2Decoder.cpp** - Implements IWICBitmapDecoder (15 methods)
- **RW2FrameDecode.cpp** - Implements IWICBitmapFrameDecode (11 methods)

### 🔷 RAW Processing
- **LibRaw Integration** - High-quality RAW to RGB conversion
- **Processing Pipeline** - open → unpack → process → output

### 🔷 System Integration
- **Registration.cpp** - Windows registry operations for codec discovery
- **install.bat / uninstall.bat** - User-friendly installation

### 🔷 Quality Assurance
- **TestDecoder.cpp** - Comprehensive testing and validation
- **Error Handling** - Proper HRESULT codes throughout

## Build Dependencies

### Required
- ✅ **Windows SDK** (for WIC headers)
  - wincodec.h
  - wincodecsdk.h
  - objbase.h

- ✅ **LibRaw** (for RAW processing)
  - Install via: `vcpkg install libraw:x64-windows`

- ✅ **CMake** 3.15+
  - For build configuration

- ✅ **Visual Studio 2019+**
  - C++17 compiler
  - Windows development tools

### Optional
- vcpkg (recommended for dependency management)
- Git (for version control)

## Runtime Dependencies

### DLL Dependencies
- **kernel32.dll** - Windows base APIs
- **ole32.dll** - COM support
- **oleaut32.dll** - Automation support
- **windowscodecs.dll** - WIC framework
- **LibRaw.dll** - RAW processing (must be distributed)

## Distribution Checklist

When distributing the codec, include:
- ✅ RW2Codec.dll
- ✅ LibRaw.dll (and its dependencies)
- ✅ install.bat
- ✅ uninstall.bat
- ✅ README.md
- ✅ QUICKSTART.md
- ✅ TROUBLESHOOTING.md

## Integration Points

### Windows Explorer
```
File → RW2Codec.dll → LibRaw → Thumbnail Cache → Explorer UI
```

### Windows Photos
```
RW2 File → WIC Framework → RW2Codec.dll → LibRaw → RGB Bitmap → Photos App
```

### Other Applications
```
Paint/Office/etc → WIC API → RW2Codec.dll → LibRaw → Image Data
```

## Registry Structure

```
HKEY_CLASSES_ROOT
└── CLSID
    ├── {8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}  # RW2 Decoder CLSID
    │   ├── (Default) = "Panasonic RW2 Decoder"
    │   └── InprocServer32
    │       ├── (Default) = "C:\path\to\RW2Codec.dll"
    │       └── ThreadingModel = "Both"
    │
    └── {7ED96837-96F0-4812-B211-F13C24117ED3}  # WIC Decoder Category
        └── Instance
            └── {8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
                ├── CLSID = "{8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}"
                ├── FriendlyName = "Panasonic RW2 Decoder"
                ├── FileExtensions = ".rw2,.RW2"
                ├── MimeTypes = "image/x-panasonic-rw2"
                ├── Vendor = "{8F3E8E61-9C1A-4B3D-A5E1-7C9F8B2D4A3C}"
                ├── Version = "1.0.0.0"
                ├── SupportsLossless = 1
                └── ContainerFormat = "{FE99CE60-F19C-433C-A3AE-00ACEFA9CA21}"
```

## Thread Safety Model

- **ThreadingModel**: "Both" (apartment and free-threaded)
- **Synchronization**: Critical sections (CRITICAL_SECTION)
- **Reference Counting**: Atomic operations (InterlockedIncrement/Decrement)
- **State Management**: Per-instance state with locks

## Memory Management

- **Smart Pointers**: std::unique_ptr for LibRaw
- **RAII Pattern**: Automatic cleanup in destructors
- **COM Reference Counting**: AddRef/Release pattern
- **LibRaw Cleanup**: Manual dcraw_clear_mem for processed images

## Error Handling

### HRESULT Mapping
```
LibRaw Success          → S_OK
File Not Found          → WINCODEC_ERR_COMPONENTNOTFOUND
Invalid File            → WINCODEC_ERR_BADIMAGE
Not Initialized         → WINCODEC_ERR_NOTINITIALIZED
Wrong State             → WINCODEC_ERR_WRONGSTATE
Palette Not Available   → WINCODEC_ERR_PALETTEUNAVAILABLE
Not Supported           → WINCODEC_ERR_UNSUPPORTEDOPERATION
Generic Failure         → E_FAIL
Invalid Argument        → E_INVALIDARG
Out of Memory           → E_OUTOFMEMORY
```

---

**Project Status: ✅ COMPLETE AND READY FOR BUILD**

All components implemented, documented, and ready for compilation and deployment.
