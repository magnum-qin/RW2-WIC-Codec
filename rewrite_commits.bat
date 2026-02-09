@echo off
echo ========================================
echo Rewrite Git History with Conventional Commits
echo ========================================
echo.
echo WARNING: This will rewrite commit history!
echo Only do this BEFORE pushing to GitHub.
echo.
echo Current commits:
echo ----------------
git log --oneline
echo.
echo Do you want to rewrite these commits with Conventional Commits format?
choice /C YN /M "Continue (Y=Yes, N=Cancel)"

if errorlevel 2 (
    echo Cancelled.
    pause
    exit /b 0
)

echo.
echo Rewriting commit history...
echo.

REM Method: Create new commits with proper messages
REM This is safer than interactive rebase

REM Step 1: Create a backup branch
git branch backup-before-rewrite

echo Created backup branch: backup-before-rewrite
echo.

REM Step 2: Reset to root
git reset --soft 1eb3dd0~1

REM Step 3: Create properly formatted commits

echo Creating commit 1/4: Initial implementation...
git add .
git commit -m "feat: initial RW2 WIC Codec implementation

Complete Windows Imaging Component decoder for Panasonic RW2 RAW files

Features:
- IWICBitmapDecoder interface implementation
- IWICBitmapFrameDecode with LibRaw integration
- COM architecture with proper reference counting
- System registration for Windows Explorer
- High-quality RAW processing (AHD interpolation, camera WB)
- 24bpp RGB output
- Automated build and installation scripts
- Comprehensive test suite

Components:
- decoder: Core WIC decoder implementation
- registration: COM and WIC system registration
- libraw: RAW processing integration
- build: CMake configuration with vcpkg
- docs: English documentation
- tests: Test program for validation

Co-Authored-By: Claude Sonnet 4.5 <noreply@anthropic.com>"

echo.
echo Creating commit 2/4: Add license...
git add LICENSE
git commit -m "chore: add MIT License

Add MIT License with LibRaw licensing information
for open source distribution"

echo.
echo Creating commit 3/4: Add documentation...
git add GITHUB_PUSH_GUIDE.md PUSH_QUICKSTART.md PUSH_SIMPLE.md
git add README_CN.md REPOSITORY_DESCRIPTIONS.md
git add push_github.bat push_to_github.bat
git commit -m "docs: add GitHub push tools and Chinese documentation

- Add Chinese README (README_CN.md)
- Add repository descriptions in CN/EN (REPOSITORY_DESCRIPTIONS.md)
- Add GitHub push guides (GITHUB_PUSH_GUIDE.md, PUSH_QUICKSTART.md)
- Add simplified push scripts (push_github.bat, push_to_github.bat)
- Fix batch file encoding issues with English interface"

echo.
echo Creating commit 4/4: Add commit conventions...
git add COMMIT_CONVENTIONS.md .gitmessage READY_TO_PUSH.md
git commit -m "docs: add Conventional Commits guide and templates

- Add COMMIT_CONVENTIONS.md with commit message standards
- Add .gitmessage template for standardized commits
- Add READY_TO_PUSH.md with final push checklist
- Configure project for conventional commit workflow"

echo.
echo ========================================
echo Success! Commits rewritten
echo ========================================
echo.
echo New commit history:
git log --oneline
echo.
echo Backup branch available at: backup-before-rewrite
echo.
echo If you want to undo this:
echo   git reset --hard backup-before-rewrite
echo.
echo To remove backup after pushing:
echo   git branch -D backup-before-rewrite
echo.
pause
