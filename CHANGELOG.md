# Changelog

All notable changes to this project are documented here. The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project uses semantic versioning.

## [Unreleased]

### Added

- Push and pull-request CI for Debug and Release builds.
- CTest smoke tests for the diagnostic executables.
- GitHub issue forms, pull-request template, Dependabot configuration, security policy, support guide, code of conduct, and third-party notices.
- vcpkg manifest, EditorConfig, and Git attributes.

### Changed

- Reworked the English, Chinese, and Japanese README files around installation, compatibility, verification, and support.
- Kept the MIT license file machine-detectable by moving LibRaw notices into a dedicated document.
- Synchronized the registered codec version with the project version.

### Removed

- Prebuilt `scripts/RW2Codec.dll` from source control. Release binaries remain available through GitHub Releases.

## [1.1.9] - 2026-06-30

### Fixed

- Made installer and ZIP dependency collection resilient to vcpkg DLL naming differences.
- Added loader fallbacks for `z.dll` and `zlib.dll`.
- Completed a successful automated v1.1.9 build and release.

## [1.1.7] - 2026-06-29

### Added

- EXIF diagnostic utility.

## [1.1.6] - 2026-06-29

### Changed

- Switched the release workflow to Ninja and configured the MSVC environment.

## [1.1.2] - 2026-06-29

### Added

- Automated tagged-release packaging.
- EXIF metadata query support.
- Embedded JPEG preview support.
- Thread-safe local loading of LibRaw runtime dependencies.
- COM boundary exception handling.

## [1.1.0] - 2026-02-27

### Added

- Inno Setup installer.
- Thumbnail support and LibRaw performance diagnostic.
- RW2 WIC frame decoding.

## [1.0.0] - 2026-02-26

### Added

- Initial x64 Windows WIC decoder for Panasonic RW2 files.
- COM registration and unregistration.
- LibRaw-backed 24-bit BGR decoding.
- CMake build and basic WIC decoder diagnostic.

[Unreleased]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.1.9...HEAD
[1.1.9]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.1.7...v1.1.9
[1.1.7]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.1.6...v1.1.7
[1.1.6]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.1.2...v1.1.6
[1.1.2]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.1.0...v1.1.2
[1.1.0]: https://github.com/magnum-qin/RW2-WIC-Codec/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/magnum-qin/RW2-WIC-Codec/releases/tag/v1.0.0
