@echo off
REM ========================================
REM Quick Push to GitHub Script
REM ========================================

echo.
echo GitHub Push Script for RW2 Codec
echo =================================
echo.

REM Check if remote exists
git remote get-url origin >nul 2>&1
if %errorLevel% equ 0 (
    echo Remote repository already configured:
    git remote get-url origin
    echo.
    goto :push
)

REM Configure remote
echo Step 1: Create repository on GitHub
echo ------------------------------------
echo.
echo 1. Go to: https://github.com/new
echo 2. Repository name: RW2-WIC-Codec
echo 3. Do NOT initialize with README/License/.gitignore
echo 4. Click "Create repository"
echo.
echo Press any key after creating the repository...
pause >nul
echo.

echo Step 2: Enter your GitHub username
echo -----------------------------------
set /p GITHUB_USER=GitHub username:

echo.
echo Step 3: Choose authentication
echo ------------------------------
echo 1 = HTTPS (Personal Access Token)
echo 2 = SSH (SSH key)
echo.
choice /C 12 /N /M "Enter choice (1 or 2): "

if errorlevel 2 (
    set "REMOTE_URL=git@github.com:%GITHUB_USER%/RW2-WIC-Codec.git"
    echo.
    echo Using SSH: %REMOTE_URL%
) else (
    set "REMOTE_URL=https://github.com/%GITHUB_USER%/RW2-WIC-Codec.git"
    echo.
    echo Using HTTPS: %REMOTE_URL%
    echo.
    echo IMPORTANT: Use Personal Access Token as password
    echo Get token at: https://github.com/settings/tokens
)

echo.
echo Adding remote repository...
git remote add origin %REMOTE_URL%

if %errorLevel% neq 0 (
    echo ERROR: Failed to add remote
    pause
    exit /b 1
)

echo Remote added successfully!
echo.

:push
echo Step 4: Push to GitHub
echo ----------------------
echo.
git log --oneline -3
echo.
echo Ready to push. Press any key...
pause >nul

git push -u origin master

if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo SUCCESS! Code pushed to GitHub
    echo ========================================
    echo.
    echo Repository URL:
    git remote get-url origin
    echo.
) else (
    echo.
    echo ========================================
    echo PUSH FAILED
    echo ========================================
    echo.
    echo Troubleshooting:
    echo - HTTPS: Use Personal Access Token, not password
    echo - SSH: Add SSH key to GitHub first
    echo - Network: Check internet connection
    echo.
)

pause
