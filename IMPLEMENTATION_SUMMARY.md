# Implementation overview

This document summarizes the implemented architecture. It is not a claim that all cameras, malformed inputs, or Windows applications have been exhaustively tested.

## Implemented WIC surface

- COM class factory and DLL lifetime management
- `IWICBitmapDecoder`
- `IWICBitmapFrameDecode`
- `IWICMetadataQueryReader`
- Embedded preview retrieval
- 24-bit BGR pixel output
- WIC decoder and `.rw2` registration

## RAW processing

LibRaw provides parsing, camera white balance, demosaicing, color conversion, and embedded-thumbnail extraction. Decoder and frame objects share the input buffer to avoid a second full RAW-file copy.

## Reliability measures

- Codec-local dependency lookup instead of changing the process-wide DLL search path
- `std::call_once` initialization for runtime dependencies
- HRESULT validation and argument checks
- C++ exception barriers on COM-facing operations
- Panasonic RW2 signature checks before decoding
- Cleanup paths for COM objects, LibRaw buffers, and registry handles

These measures reduce risk but do not prove the absence of memory-safety, loader, or malformed-input vulnerabilities. Report suspected issues through [SECURITY.md](SECURITY.md).

## Verification model

| Layer | Current coverage |
| --- | --- |
| Push and pull request | Windows x64 Debug/Release build |
| Automated CTest | Diagnostic executable startup |
| Tagged release | Installer and portable ZIP packaging |
| Manual integration | WIC decode, EXIF, Explorer, Photos, install/uninstall |

Camera samples are not committed because RAW files are large, may have restrictive redistribution terms, and can contain sensitive metadata. Future tests should use explicitly licensed fixtures.

## Current constraints

- Windows x64 only
- Administrator registration
- 8-bit sRGB output
- Camera support inherited from the bundled LibRaw release
- No code signing in the current release workflow

See [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) for component ownership and [CONTRIBUTING.md](CONTRIBUTING.md) for the validation workflow.
