# 📤 推送到 GitHub 指南

## ✅ 准备工作已完成

您的项目已经完成了本地 Git 初始化：
- ✅ Git 仓库已创建
- ✅ 所有文件已提交（28个文件，4000+行代码）
- ✅ LICENSE 文件已添加
- ✅ .gitignore 已配置

---

## 🚀 推送到 GitHub 的步骤

### 方法 1: 使用 GitHub 网页创建仓库（推荐）

#### 步骤 1: 在 GitHub 上创建新仓库

1. **登录 GitHub**: https://github.com

2. **创建新仓库**:
   - 点击右上角的 `+` → `New repository`
   - 或访问: https://github.com/new

3. **填写仓库信息**:
   ```
   Repository name: RW2-WIC-Codec
   Description: Windows Imaging Component (WIC) codec for Panasonic RW2 RAW files

   ⚪ Public  (推荐 - 开源项目)
   ⚫ Private (如果不想公开)

   ❌ 不要勾选 "Initialize this repository with:"
      - 不要勾选 README
      - 不要勾选 .gitignore
      - 不要勾选 License
   （因为我们已经有这些文件了）
   ```

4. **点击**: `Create repository`

#### 步骤 2: 连接本地仓库到 GitHub

GitHub 会显示推送指令，但您可以直接使用下面的命令：

```bash
# 在 RW2Codec 目录下运行：

# 添加远程仓库（替换 YOUR_USERNAME 为您的 GitHub 用户名）
git remote add origin https://github.com/YOUR_USERNAME/RW2-WIC-Codec.git

# 或使用 SSH（如果已配置 SSH 密钥）
git remote add origin git@github.com:YOUR_USERNAME/RW2-WIC-Codec.git

# 推送到 GitHub
git push -u origin master
```

#### 步骤 3: 输入凭据

- 如果使用 HTTPS：需要输入 GitHub 用户名和个人访问令牌（Personal Access Token）
- 如果使用 SSH：需要之前配置好 SSH 密钥

---

### 方法 2: 使用 GitHub CLI（gh）

如果您安装了 GitHub CLI：

```bash
# 在 RW2Codec 目录下运行：

# 登录 GitHub
gh auth login

# 创建仓库并推送
gh repo create RW2-WIC-Codec --public --source=. --remote=origin --push

# 或创建私有仓库
gh repo create RW2-WIC-Codec --private --source=. --remote=origin --push
```

---

## 🔑 GitHub 认证设置

### 选项 1: 个人访问令牌（Personal Access Token）

从 2021 年起，GitHub 不再支持密码认证，需要使用 PAT：

1. **生成 Token**:
   - 访问: https://github.com/settings/tokens
   - 点击 `Generate new token` → `Generate new token (classic)`
   - Note: `RW2 Codec Development`
   - Expiration: 选择有效期
   - 勾选权限: `repo` (完整的仓库访问权限)
   - 点击 `Generate token`
   - **复制并保存** Token（只显示一次！）

2. **使用 Token**:
   ```bash
   # 推送时，用户名输入您的 GitHub 用户名
   # 密码输入刚才复制的 Token
   git push -u origin master
   ```

### 选项 2: SSH 密钥（更方便）

1. **生成 SSH 密钥** (如果还没有):
   ```bash
   ssh-keygen -t ed25519 -C "your_email@example.com"
   # 按 Enter 使用默认位置
   # 可以设置密码或直接按 Enter
   ```

2. **添加到 SSH Agent**:
   ```bash
   eval "$(ssh-agent -s)"
   ssh-add ~/.ssh/id_ed25519
   ```

3. **添加到 GitHub**:
   ```bash
   # 复制公钥
   cat ~/.ssh/id_ed25519.pub
   # 或在 Windows 上
   clip < ~/.ssh/id_ed25519.pub
   ```

   - 访问: https://github.com/settings/keys
   - 点击 `New SSH key`
   - Title: `My Computer`
   - Key: 粘贴刚才复制的内容
   - 点击 `Add SSH key`

4. **测试连接**:
   ```bash
   ssh -T git@github.com
   # 应该看到: Hi username! You've successfully authenticated...
   ```

---

## 📝 快速命令参考

### 完整推送流程（使用 HTTPS）

```bash
# 1. 进入项目目录
cd /c/Users/qhz00/Projects/RW2Codec

# 2. 添加远程仓库（替换 YOUR_USERNAME）
git remote add origin https://github.com/YOUR_USERNAME/RW2-WIC-Codec.git

# 3. 验证远程仓库
git remote -v

# 4. 推送到 GitHub
git push -u origin master

# 5. 输入 GitHub 用户名和 Personal Access Token
```

### 完整推送流程（使用 SSH）

```bash
# 1. 进入项目目录
cd /c/Users/qhz00/Projects/RW2Codec

# 2. 添加远程仓库（替换 YOUR_USERNAME）
git remote add origin git@github.com:YOUR_USERNAME/RW2-WIC-Codec.git

# 3. 推送到 GitHub
git push -u origin master
```

---

## 🎨 推荐的仓库设置

创建仓库后，在 GitHub 网页上进行以下设置：

### 1. 添加 Topics（标签）

在仓库页面点击 ⚙️ 设置 Topics：
- `wic`
- `windows-imaging-component`
- `raw-images`
- `panasonic`
- `rw2`
- `codec`
- `windows`
- `cpp`
- `libraw`

### 2. 编辑 About

- Description: `Windows Imaging Component (WIC) codec for Panasonic RW2 RAW files - Native RW2 support in Windows Explorer and Photos app`
- Website: (如果有演示视频或博客)

### 3. 添加 README Badges（可选）

在 README.md 顶部添加徽章：

```markdown
# RW2 WIC Codec

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)
![Status](https://img.shields.io/badge/status-Active-success.svg)

Windows Imaging Component (WIC) codec for Panasonic RW2 RAW files...
```

---

## 📦 后续推送

以后修改代码后，推送更新：

```bash
# 1. 查看修改
git status

# 2. 添加修改的文件
git add .

# 3. 提交
git commit -m "描述您的修改"

# 4. 推送
git push
```

---

## 🌟 推荐的 GitHub Actions（可选）

创建 `.github/workflows/build.yml` 启用自动构建：

```yaml
name: Build

on: [push, pull_request]

jobs:
  build:
    runs-on: windows-latest

    steps:
    - uses: actions/checkout@v3

    - name: Setup vcpkg
      uses: lukka/run-vcpkg@v11

    - name: Build
      run: |
        mkdir build
        cd build
        cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
        cmake --build . --config Release
```

---

## ✅ 验证推送成功

推送成功后：

1. **访问您的仓库**: `https://github.com/YOUR_USERNAME/RW2-WIC-Codec`
2. **应该能看到**:
   - 所有源代码文件
   - README.md 自动渲染
   - 28 个文件
   - 2 次提交

3. **分享给他人**:
   ```
   git clone https://github.com/YOUR_USERNAME/RW2-WIC-Codec.git
   ```

---

## 🎯 现在就开始！

**您需要做的：**

1. 访问 https://github.com/new 创建仓库
2. 运行下面的命令（替换 YOUR_USERNAME）：

```bash
cd /c/Users/qhz00/Projects/RW2Codec
git remote add origin https://github.com/YOUR_USERNAME/RW2-WIC-Codec.git
git push -u origin master
```

3. 完成！🎉

---

有任何问题随时告诉我！
