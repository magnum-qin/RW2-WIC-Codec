# RW2 WIC Codec v1.1.9

Version 1.1.9 is the current stable release for 64-bit Windows 10 and Windows 11.

## Highlights

- Native EXIF metadata access through `IWICMetadataQueryReader`
- Embedded JPEG preview retrieval through `GetPreview`
- Shared RAW input buffers between decoder and frame objects
- Exception barriers around COM-facing operations
- Thread-safe, codec-local loading of LibRaw dependencies
- Panasonic RW2 signature validation
- Automated Windows x64 installer and ZIP packaging

## Packaging fixes in v1.1.9

- Collect runtime dependencies by wildcard so vcpkg naming changes do not silently omit required DLLs.
- Try `zlib1.dll`, `z.dll`, and `zlib.dll` when loading the zlib dependency.
- Produce both an Inno Setup installer and a portable archive.

## Downloads

- `RW2Codec_Setup_v1.1.9.exe` — recommended installer
- `RW2Codec-v1.1.9-x64.zip` — portable package with registration scripts

Download only from the [v1.1.9 release page](https://github.com/magnum-qin/RW2-WIC-Codec/releases/tag/v1.1.9). The artifacts are not code-signed; compare the SHA-256 digest displayed by GitHub before running them.

## Upgrade

Uninstall an older portable registration before moving or deleting its DLLs. Then install v1.1.9 and restart File Explorer. Installer-based installations can be upgraded by running the new installer.

## Known limitations

- x64 Windows only
- Administrator privileges are required for COM registration
- Camera compatibility depends on the bundled LibRaw version
- Full decoder validation requires a real, legally shareable RW2 sample
- Output is currently 8-bit, 24-bit BGR in sRGB

See [CHANGELOG.md](CHANGELOG.md) for version history and [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for diagnostics.
