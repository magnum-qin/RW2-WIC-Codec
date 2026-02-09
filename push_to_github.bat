@echo off
chcp 65001 >nul
echo ========================================
echo Push RW2 Codec to GitHub
echo ========================================
echo.

REM Check if remote repository is already configured
git remote -v | findstr "origin" >nul 2>&1
if %errorLevel% equ 0 (
    echo [INFO] Remote repository already configured
    git remote -v
    echo.
    echo Do you want to push to existing remote?
    choice /C YN /M "Choice (Y=Yes, N=Reconfigure)"
    if errorlevel 2 goto :configure
    goto :push
)

:configure
echo.
echo ========================================
echo Configure GitHub Remote Repository
echo ========================================
echo.
echo Please create a new repository on GitHub first:
echo   1. Visit: https://github.com/new
echo   2. Repository name: RW2-WIC-Codec
echo   3. Do NOT check "Initialize this repository with..."
echo   4. Click "Create repository"
echo.
pause
echo.

REM Remove existing origin if any
git remote remove origin >nul 2>&1

echo Please enter your GitHub username:
set /p GITHUB_USER=Username:

echo.
echo Choose authentication method:
echo   1. HTTPS (requires Personal Access Token)
echo   2. SSH (requires SSH key configured)
echo.
choice /C 12 /M "Choose"

if errorlevel 2 (
    REM SSH
    set "REMOTE_URL=git@github.com:%GITHUB_USER%/RW2-WIC-Codec.git"
    echo.
    echo Using SSH URL: %REMOTE_URL%
) else (
    REM HTTPS
    set "REMOTE_URL=https://github.com/%GITHUB_USER%/RW2-WIC-Codec.git"
    echo.
    echo Using HTTPS URL: %REMOTE_URL%
    echo.
    echo [IMPORTANT] When pushing, you need to enter:
    echo   Username: %GITHUB_USER%
    echo   Password: Your Personal Access Token (NOT your password!)
    echo.
    echo How to get Personal Access Token:
    echo   1. Visit: https://github.com/settings/tokens
    echo   2. Click: Generate new token (classic)
    echo   3. Check: repo (full permissions)
    echo   4. Generate and copy the token
    echo.
    pause
)

echo.
echo Adding remote repository...
git remote add origin %REMOTE_URL%

if %errorLevel% neq 0 (
    echo.
    echo [ERROR] Failed to add remote repository
    pause
    exit /b 1
)

echo [SUCCESS] Remote repository configured
echo.

:push
echo ========================================
echo Push to GitHub
echo ========================================
echo.
echo Current commits:
git log --oneline -3
echo.

echo Ready to push to GitHub...
echo.
pause

git push -u origin master

if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo Success! Repository pushed to GitHub
    echo ========================================
    echo.
    echo Your repository URL:
    git remote get-url origin
    echo.
    echo Open in browser to view your code!
    echo.
) else (
    echo.
    echo ========================================
    echo Push Failed
    echo ========================================
    echo.
    echo Common issues:
    echo   1. HTTPS: Make sure to use Personal Access Token, not password
    echo   2. SSH: Make sure SSH key is added to GitHub
    echo   3. Network: Check network connection and proxy settings
    echo.
    echo See detailed help: GITHUB_PUSH_GUIDE.md
    echo.
)

pause
