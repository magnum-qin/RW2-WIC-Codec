@echo off
echo ========================================
echo Rewrite Git History with Conventional Commits
echo ========================================
echo.
echo IMPORTANT: This will rewrite ALL commit history!
echo.
echo Current commits:
git log --oneline
echo.
echo After rewriting, you will need to use "git push --force"
echo This will overwrite the history on GitHub.
echo.
echo ⚠️  WARNING:
echo   - Anyone who has cloned the repo will have conflicts
echo   - GitHub links to old commits will break
echo   - This is SAFE if you are the only user
echo.
echo Are you the only person using this repository?
choice /C YN /M "Continue with rewrite (Y=Yes, N=Cancel)"

if errorlevel 2 (
    echo.
    echo Cancelled. No changes made.
    pause
    exit /b 0
)

echo.
echo Creating backup branch...
git branch backup-before-conventional-commits

echo.
echo Step 1: Resetting to initial state...
git reset --soft 1eb3dd0

echo.
echo Step 2: Creating properly formatted commits...
echo.

REM Commit 1: Initial implementation
echo Creating commit 1/5: Initial implementation...
git add CMakeLists.txt RW2Codec.def .gitignore
git add include/ src/ tests/ scripts/
git add README.md QUICKSTART.md IMPLEMENTATION_SUMMARY.md PROJECT_STRUCTURE.md
git add CHANGELOG.md CONTRIBUTING.md TROUBLESHOOTING.md
git add build.bat "项目规划.txt"

git commit -m "feat: initial RW2 WIC Codec implementation

Implement complete Windows Imaging Component decoder for Panasonic RW2 RAW files

Components:
- decoder: IWICBitmapDecoder and IWICBitmapFrameDecode implementation
- libraw: High-quality RAW processing with AHD interpolation
- registration: COM and WIC system registration
- build: CMake configuration with vcpkg integration
- tests: Comprehensive test suite with validation program

Features:
- System-wide RW2 support in Windows Explorer and Photos app
- 24bpp RGB output with camera white balance
- Thumbnail display in File Explorer
- High-quality demosaicing and color processing
- Thread-safe COM architecture
- Automated installation scripts

Documentation:
- Complete English documentation (README.md, guides)
- Build instructions and troubleshooting
- Contributing guidelines
- Implementation summary and project structure

Technical details:
- C++17 with proper COM reference counting
- LibRaw integration for professional RAW processing
- Registry-based system integration
- Support for all Panasonic Lumix cameras (S/G/GH/GX series)

Co-Authored-By: Claude Sonnet 4.5 <noreply@anthropic.com>"

if %errorLevel% neq 0 (
    echo ERROR: First commit failed
    pause
    exit /b 1
)

REM Commit 2: License
echo Creating commit 2/5: Add license...
git add LICENSE
git commit -m "chore: add MIT License

Add MIT License with LibRaw licensing information for open source distribution"

if %errorLevel% neq 0 (
    echo ERROR: License commit failed
    pause
    exit /b 1
)

REM Commit 3: GitHub push tools and Chinese docs
echo Creating commit 3/5: Add GitHub tools and Chinese documentation...
git add GITHUB_PUSH_GUIDE.md PUSH_QUICKSTART.md PUSH_SIMPLE.md
git add README_CN.md REPOSITORY_DESCRIPTIONS.md
git add push_github.bat push_to_github.bat

git commit -m "docs: add GitHub push tools and Chinese documentation

GitHub Integration:
- Add comprehensive push guide (GITHUB_PUSH_GUIDE.md)
- Add quick start guide (PUSH_QUICKSTART.md)
- Add simple command reference (PUSH_SIMPLE.md)
- Add automated push scripts (push_github.bat, push_to_github.bat)
- Fix batch file encoding issues with English interface

Chinese Documentation:
- Add complete Chinese README (README_CN.md)
- Add repository descriptions in CN/EN (REPOSITORY_DESCRIPTIONS.md)
- Provide bilingual documentation for wider accessibility

Tools:
- Interactive push script with HTTPS/SSH selection
- Personal Access Token setup guide
- Troubleshooting for common push issues"

if %errorLevel% neq 0 (
    echo ERROR: Documentation commit failed
    pause
    exit /b 1
)

REM Commit 4: Conventional Commits setup
echo Creating commit 4/5: Add Conventional Commits support...
git add COMMIT_CONVENTIONS.md .gitmessage READY_TO_PUSH.md
git add CONTRIBUTING.md rewrite_commits.bat

git commit -m "docs: add Conventional Commits guide and tooling

Setup:
- Add comprehensive Conventional Commits guide (COMMIT_CONVENTIONS.md)
- Add Git commit message template (.gitmessage)
- Configure local Git to use commit template
- Add push readiness checklist (READY_TO_PUSH.md)

Updates:
- Update CONTRIBUTING.md with commit message standards
- Add examples for feat, fix, docs, perf, refactor, etc.
- Add commit rewrite script for history cleanup

Benefits:
- Standardized commit messages
- Automated CHANGELOG generation
- Easier navigation of project history
- Better collaboration practices"

if %errorLevel% neq 0 (
    echo ERROR: Conventional Commits commit failed
    pause
    exit /b 1
)

REM Commit 5: Explanation
echo Creating commit 5/5: Add adoption explanation...
git add ABOUT_COMMITS.md

git commit -m "docs: add explanation about Conventional Commits adoption

Add documentation explaining:
- Why Conventional Commits format is being adopted
- How to use the new commit format going forward
- Configuration of Git commit template
- Examples and best practices

Provides guidance for:
- New contributors understanding commit standards
- Team members adopting the convention
- Future CHANGELOG automation"

if %errorLevel% neq 0 (
    echo ERROR: Final commit failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo ✓ Success! All commits rewritten
echo ========================================
echo.
echo New commit history:
git log --oneline --all --graph --decorate -10
echo.
echo Backup branch created: backup-before-conventional-commits
echo.
echo ========================================
echo Next Steps:
echo ========================================
echo.
echo 1. Review the new commit history above
echo.
echo 2. If satisfied, force push to GitHub:
echo    git push --force origin master
echo.
echo 3. If you want to undo this:
echo    git reset --hard backup-before-conventional-commits
echo    git branch -D master
echo    git checkout -b master backup-before-conventional-commits
echo.
echo ⚠️  IMPORTANT: Force push will rewrite GitHub history!
echo    - Old commit links will break
echo    - Anyone who cloned will need to re-clone
echo    - Only do this if you're the sole user
echo.
pause
