# Contributing to RW2 WIC Codec

Thank you for helping improve RW2 WIC Codec. Bug reports, camera compatibility results, documentation fixes, tests, and focused code changes are welcome.

All participation is subject to the [Code of Conduct](CODE_OF_CONDUCT.md).

## Before you start

- Search existing issues and pull requests.
- Open an issue before starting a large or compatibility-sensitive change.
- Report vulnerabilities privately as described in [SECURITY.md](SECURITY.md).
- Do not publish an RW2 sample until you have checked it for location, camera serial number, author, or other sensitive metadata.

## Development setup

You need Windows 10 or 11 x64, Visual Studio 2022 with **Desktop development with C++**, CMake 3.15 or later, Git, and vcpkg.

```batch
git clone https://github.com/magnum-qin/RW2-WIC-Codec.git
cd RW2-WIC-Codec
vcpkg install --triplet x64-windows
cmake -S . -B build -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DBUILD_TESTING=ON
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

See [BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md) and [TROUBLESHOOTING.md](TROUBLESHOOTING.md) if configuration fails.

## Making a change

1. Create a focused branch.
2. Follow the existing C++17 style: four-space indentation, PascalCase COM methods and classes, and `m_camelCase` private members.
3. Keep exception handling at COM boundaries and preserve HRESULT semantics.
4. Add or update tests when behavior changes.
5. Update README, changelog, release notes, installer version, and CMake version together when preparing a release.
6. Build both Debug and Release.

Use Conventional Commits when practical:

```text
feat(decoder): add embedded preview fallback
fix(registration): correct WIC pattern metadata
docs(readme): clarify unsigned installer warning
test(metadata): cover ISO variant conversion
```

## Testing

### Automated checks

```batch
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The automated tests are smoke tests that do not require registering the codec or committing camera samples.

### Manual integration checks

For decoder or registration changes, test with legally shareable RW2 files from more than one camera when possible:

```batch
TestDecoder.exe C:\Photos\sample.rw2
TestExif.exe C:\Photos\sample.rw2
TestPerf.exe C:\Photos\sample.rw2
```

Also verify installation, Explorer thumbnails, Windows Photos, and clean uninstallation. Never run registration tests on a production system unless you understand the registry changes.

## Pull requests

Complete the pull-request template, explain user-visible behavior, list the tested Windows and camera combinations, and include relevant command output. Keep unrelated refactors out of the same pull request.

By contributing, you agree that your contribution is licensed under the repository's [MIT License](LICENSE).
