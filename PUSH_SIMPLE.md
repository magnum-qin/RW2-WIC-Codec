# GitHub Push - Simple Commands

## Quick Push (Copy and Paste)

Replace `YOUR_USERNAME` with your GitHub username:

```bash
# Step 1: Add remote repository
git remote add origin https://github.com/YOUR_USERNAME/RW2-WIC-Codec.git

# Step 2: Push to GitHub
git push -u origin master
```

When prompted:
- **Username**: Your GitHub username
- **Password**: Personal Access Token (NOT your password!)

## Get Personal Access Token

1. Visit: https://github.com/settings/tokens
2. Click: "Generate new token (classic)"
3. Select scopes: Check "repo"
4. Click: "Generate token"
5. Copy the token immediately (it won't show again!)

## Common Commands

### Check remote configuration
```bash
git remote -v
```

### Remove remote (if needed)
```bash
git remote remove origin
```

### Push future updates
```bash
git add .
git commit -m "Your commit message"
git push
```

## Alternative: Use GitHub Desktop

1. Download: https://desktop.github.com/
2. Open GitHub Desktop
3. File → Add Local Repository → Select RW2Codec folder
4. Click "Publish repository"
5. Done!

## Need Help?

- See: GITHUB_PUSH_GUIDE.md (detailed guide)
- See: PUSH_QUICKSTART.md (quick reference)
- Or ask me!
