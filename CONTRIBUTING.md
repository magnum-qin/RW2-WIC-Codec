# Contributing to RW2 WIC Codec

Thank you for considering contributing to the RW2 WIC Codec project! This document provides guidelines for contributing.

## How to Contribute

### Reporting Bugs

If you find a bug, please open an issue with:

1. **Clear title** - Briefly describe the issue
2. **Environment details**:
   - Windows version (10/11)
   - Build version of the codec
   - Camera model that produced the RW2 file
3. **Steps to reproduce**:
   - What did you do?
   - What did you expect to happen?
   - What actually happened?
4. **Sample file** (if possible):
   - A sample RW2 file that demonstrates the issue
   - Or at least the camera model and settings

### Suggesting Features

Feature suggestions are welcome! Please:

1. Check if the feature has already been requested
2. Explain the use case - why would this be useful?
3. Describe the expected behavior
4. Consider implementation complexity

### Code Contributions

#### Before You Start

1. **Check existing issues** - Someone might already be working on it
2. **Discuss major changes** - Open an issue first for large features
3. **Follow coding standards** - Match the existing code style

#### Development Setup

1. **Fork the repository**
2. **Clone your fork**:
   ```batch
   git clone https://github.com/your-username/RW2Codec.git
   ```
3. **Install dependencies**:
   ```batch
   vcpkg install libraw:x64-windows
   ```
4. **Build the project**:
   ```batch
   build.bat
   ```

#### Commit Message Standards

**Use Conventional Commits format:**

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code formatting
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Testing changes
- `build`: Build system changes
- `ci`: CI configuration
- `chore`: Other changes

**Examples:**
```bash
feat(decoder): add embedded thumbnail extraction
fix(registration): correct registry key path
docs(readme): add Chinese translation
perf(libraw): optimize memory allocation
```

See [COMMIT_CONVENTIONS.md](COMMIT_CONVENTIONS.md) for detailed guide.

#### Coding Standards

- **C++ Standard**: C++17
- **Naming Conventions**:
  - Classes: PascalCase (e.g., `RW2Decoder`)
  - Methods: PascalCase for COM interfaces (e.g., `GetSize()`)
  - Private members: m_camelCase (e.g., `m_refCount`)
  - Local variables: camelCase
- **Formatting**:
  - 4 spaces for indentation (no tabs)
  - Opening braces on same line for methods
  - Opening braces on new line for classes/namespaces
- **Comments**:
  - Document public interfaces
  - Explain "why", not "what" for complex logic
  - Use `//` for single-line, `/* */` for multi-line

#### Testing

Before submitting a pull request:

1. **Build in both Debug and Release**:
   ```batch
   cmake --build . --config Debug
   cmake --build . --config Release
   ```

2. **Test with multiple RW2 files**:
   - Different camera models
   - Different resolutions
   - Different orientations

3. **Run the test program**:
   ```batch
   TestDecoder.exe path\to\test.rw2
   ```

4. **Verify system integration**:
   - Install the codec
   - Check File Explorer thumbnails
   - Open files in Photos app
   - Uninstall cleanly

#### Pull Request Process

1. **Create a feature branch**:
   ```batch
   git checkout -b feature/my-feature
   ```

2. **Make your changes**:
   - Write clean, documented code
   - Add tests if applicable
   - Update documentation

3. **Commit with clear messages**:
   ```
   Add embedded thumbnail extraction

   - Implement IWICBitmapDecoder::GetThumbnail()
   - Extract JPEG thumbnail from EXIF data
   - Fallback to full decode if no thumbnail
   - Add tests for thumbnail extraction
   ```

4. **Push to your fork**:
   ```batch
   git push origin feature/my-feature
   ```

5. **Open a pull request**:
   - Describe what changed and why
   - Reference any related issues
   - Include test results

6. **Respond to feedback**:
   - Address review comments
   - Update as needed

### Documentation

Documentation improvements are always welcome:

- Fix typos or unclear explanations
- Add examples or use cases
- Improve installation instructions
- Translate to other languages

## Code of Conduct

### Our Standards

- **Be respectful** - Treat everyone with respect
- **Be constructive** - Provide helpful feedback
- **Be inclusive** - Welcome newcomers
- **Be patient** - Not everyone has the same experience level

### Unacceptable Behavior

- Harassment or discriminatory language
- Trolling or insulting comments
- Publishing others' private information
- Other conduct that would be inappropriate in a professional setting

## Questions?

If you have questions about contributing:

1. Check the README and documentation
2. Search existing issues
3. Open a new issue with the "question" label

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).

---

Thank you for helping make RW2 WIC Codec better! 🎉
