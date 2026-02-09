# Conventional Commits 规范指南

## 📋 什么是 Conventional Commits？

Conventional Commits 是一种提交信息的标准化格式，使提交历史更清晰、可读，并且能自动生成 CHANGELOG。

### 格式

```
<type>(<scope>): <subject>

<body>

<footer>
```

---

## 🏷️ 提交类型 (Type)

### 主要类型

| Type | 说明 | 示例 |
|------|------|------|
| `feat` | 新功能 | `feat(decoder): add RW2 file signature validation` |
| `fix` | Bug修复 | `fix(registration): correct registry key path` |
| `docs` | 文档变更 | `docs(readme): add Chinese translation` |
| `style` | 代码格式（不影响功能） | `style(decoder): format code with clang-format` |
| `refactor` | 重构（既不是新功能也不是修复） | `refactor(libraw): optimize memory allocation` |
| `perf` | 性能优化 | `perf(decoder): cache processed images` |
| `test` | 测试相关 | `test(decoder): add unit tests for RW2Decoder` |
| `build` | 构建系统或外部依赖 | `build(cmake): add LibRaw dependency` |
| `ci` | CI配置文件和脚本 | `ci(github): add build workflow` |
| `chore` | 其他杂项 | `chore(git): update .gitignore` |
| `revert` | 回滚之前的提交 | `revert: feat(decoder): add thumbnail support` |

### 可选的作用域 (Scope)

作用域说明提交影响的范围，例如：
- `decoder` - 解码器相关
- `registration` - 注册相关
- `libraw` - LibRaw集成
- `docs` - 文档
- `build` - 构建系统
- `tests` - 测试

---

## ✍️ 提交信息示例

### 好的提交信息 ✅

```bash
feat(decoder): implement IWICBitmapDecoder interface

- Add QueryCapability for RW2 signature validation
- Implement Initialize method with stream handling
- Add GetFrame method returning IWICBitmapFrameDecode
- Support single-frame RAW images

Closes #1
```

```bash
fix(registration): correct registry key for WIC decoder category

The previous GUID was incorrect, causing the codec not to be
discovered by Windows Explorer.

Fixes #5
```

```bash
docs(readme): add installation instructions in Chinese

- Add README_CN.md with complete Chinese documentation
- Add BUILD_GUIDE_CN.md for build instructions
- Update main README with language links
```

```bash
perf(libraw): optimize RAW processing with cached results

Reduce processing time by 40% through:
- Cache processed images in memory
- Lazy loading of RAW data
- Reuse LibRaw processor instance
```

### 不好的提交信息 ❌

```bash
# 太简短，缺少上下文
update code

# 没有类型前缀
Added new feature for thumbnails

# 描述不清晰
fix bug

# 混合多个改动
feat: add decoder and fix registration and update docs
```

---

## 🔧 为项目重写提交历史

让我用 Conventional Commits 规范重写提交信息：

### 当前提交历史

```
6df6e3a - Add documentation and GitHub push tools
2c48c3f - Add MIT License
1eb3dd0 - Initial commit: RW2 WIC Codec implementation
```

### 应该改为

```
6df6e3a - docs: add GitHub push tools and Chinese documentation
2c48c3f - chore: add MIT License
1eb3dd0 - feat: initial RW2 WIC Codec implementation
```

---

## 📝 项目提交规范模板

### 新功能

```bash
feat(decoder): add embedded thumbnail extraction

Implement IWICBitmapDecoder::GetThumbnail() to extract
embedded JPEG thumbnails from RW2 files, improving
preview generation speed by 80%.
```

### Bug修复

```bash
fix(libraw): handle corrupted RW2 files gracefully

Add proper error handling for LibRaw failures:
- Check return codes from all LibRaw functions
- Return WINCODEC_ERR_BADIMAGE for corrupted files
- Prevent memory leaks on error paths

Fixes #12
```

### 文档更新

```bash
docs(contributing): add contribution guidelines

- Add CONTRIBUTING.md with coding standards
- Add pull request template
- Add issue templates for bugs and features
```

### 构建系统

```bash
build(cmake): add support for static LibRaw linking

- Add option BUILD_SHARED_LIBS
- Configure for both static and dynamic linking
- Update vcpkg dependencies
```

### 性能优化

```bash
perf(decoder): implement parallel processing for large images

Use OpenMP to parallelize demosaicing for images > 20MP,
reducing processing time from 3s to 1.2s on quad-core CPUs.
```

---

## 🚀 如何应用到当前项目

### 选项 1: 使用 git rebase（重写历史）⚠️

**警告**: 仅在推送到GitHub之前使用！

```bash
# 交互式rebase最近3次提交
git rebase -i HEAD~3

# 在编辑器中将 'pick' 改为 'reword'，然后保存
# 逐个修改每次提交的信息
```

### 选项 2: 创建新的规范提交（推荐）✅

从现在开始使用规范格式：

```bash
# 示例：添加新功能
git commit -m "feat(decoder): add EXIF metadata support

Implement IWICMetadataQueryReader to expose EXIF data:
- Camera model
- ISO, aperture, shutter speed
- Capture date and time"

# 示例：修复Bug
git commit -m "fix(build): resolve LibRaw linking error on Windows 11

Update CMakeLists.txt to use correct library paths
for Windows 11 SDK."
```

---

## 📦 提交信息模板

### 为Git配置提交模板

创建 `.gitmessage` 文件：

```bash
# <type>(<scope>): <subject>
#
# <body>
#
# <footer>
#
# Types: feat, fix, docs, style, refactor, perf, test, build, ci, chore, revert
# Scope: decoder, registration, libraw, docs, build, tests, etc.
#
# Subject: imperative, lowercase, no period
# Body: what and why, not how
# Footer: breaking changes, issue references
```

配置Git使用模板：

```bash
git config --local commit.template .gitmessage
```

---

## 🤖 自动化工具

### Commitizen (推荐)

安装交互式提交工具：

```bash
# 安装 commitizen
npm install -g commitizen cz-conventional-changelog

# 初始化项目
commitizen init cz-conventional-changelog --save-dev --save-exact

# 使用
git cz  # 代替 git commit
```

### Commitlint

验证提交信息格式：

```bash
# 安装
npm install --save-dev @commitlint/{config-conventional,cli}

# 配置 commitlint.config.js
echo "module.exports = {extends: ['@commitlint/config-conventional']}" > commitlint.config.js

# Git hook
npm install --save-dev husky
npx husky install
npx husky add .husky/commit-msg 'npx --no -- commitlint --edit "$1"'
```

---

## 📚 未来提交示例

### 功能开发

```bash
feat(decoder): implement color profile support
feat(tests): add integration tests for RW2 files
feat(api): expose decoder configuration options
```

### Bug修复

```bash
fix(decoder): handle zero-size images correctly
fix(registration): escape special characters in registry paths
fix(libraw): prevent memory leak in error handling
```

### 文档

```bash
docs(readme): update installation instructions
docs(api): add API reference documentation
docs(examples): add usage examples
```

### 性能

```bash
perf(decoder): reduce memory usage by 30%
perf(libraw): optimize buffer allocation
```

### 重构

```bash
refactor(decoder): extract common validation logic
refactor(com): simplify reference counting
```

---

## 🎯 对当前项目的建议

### 立即应用

1. ✅ 创建 `.gitmessage` 提交模板
2. ✅ 从下一次提交开始使用规范格式
3. ✅ 更新 CONTRIBUTING.md 说明提交规范

### 可选操作

1. ⚠️ 重写现有提交历史（仅在未推送时）
2. 🔧 配置 Commitizen 或 Commitlint
3. 📝 添加提交信息检查的Git hook

---

## 🔄 是否要重写提交历史？

如果您还没有推送到GitHub，我可以帮您：

1. **重写提交信息** - 使用规范格式
2. **创建提交模板** - 未来提交自动提示
3. **配置工具** - 自动化验证

请告诉我您的选择！
