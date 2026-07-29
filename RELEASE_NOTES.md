# RW2 WIC Codec v1.1.10

Version 1.1.10 is the current stable release for 64-bit Windows 10 and Windows 11.

## Highlights

- Native EXIF metadata access through `IWICMetadataQueryReader`
- Embedded JPEG preview retrieval through `GetPreview`
- Shared RAW input buffers between decoder and frame objects
- Exception barriers around COM-facing operations
- Thread-safe, codec-local loading of LibRaw dependencies
- Panasonic RW2 signature validation
- Automated Windows x64 installer and ZIP packaging

## What changed in v1.1.10

- Added reproducible vcpkg dependency management with a pinned baseline.
- Added Windows Debug and Release CI, CTest smoke tests, and CodeQL analysis.
- Added contributor, security, support, issue, and pull-request workflows.
- Added project and dependency license notices to generated packages.
- Synchronized installer, registry, CMake, and package version metadata.
- Removed prebuilt codec binaries from source control; release binaries remain available here.

## Downloads

- `RW2Codec_Setup_v1.1.10.exe` — recommended installer
- `RW2Codec-v1.1.10-x64.zip` — portable package with registration scripts

Download only from the [v1.1.10 release page](https://github.com/magnum-qin/RW2-WIC-Codec/releases/tag/v1.1.10). The artifacts are not code-signed; compare the SHA-256 digest displayed by GitHub before running them.

## Upgrade

Uninstall an older portable registration before moving or deleting its DLLs. Then install v1.1.10 and restart File Explorer. Installer-based installations can be upgraded by running the new installer.

## Known limitations

- x64 Windows only
- Administrator privileges are required for COM registration
- Camera compatibility depends on the bundled LibRaw version
- Full decoder validation requires a real, legally shareable RW2 sample
- Output is currently 8-bit, 24-bit BGR in sRGB

See [CHANGELOG.md](CHANGELOG.md) for version history and [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for diagnostics.
