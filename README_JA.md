# RW2 WIC Codec - Windows向けパナソニックRAW形式（.rw2）ネイティブデコーダー

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11%20(x64)-blue.svg)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)]()

> Windows Imaging Component (WIC) デコーダーで、Windowsシステム全体でパナソニックの `.rw2` RAWファイルをネイティブに表示・ロードできるようにします。

[English](README.md) | [简体中文](README_CN.md) | **日本語**

---

## 📷 概要

**RW2 WIC Codec** は、パナソニックのLumixカメラで撮影されたRAW画像（`.rw2`）ファイルをWindows OSで直接扱えるようにする高性能WICデコーダーです。

本プラグインをインストールすると、以下のようなWindowsシステム標準の画像機能で直接RW2ファイルを表示できます：
- **エクスプローラー** でのサムネイル・プレビュー表示
- **Windows フォト** アプリでのダイレクト表示と拡大縮小
- **ペイント** や **Microsoft Office** での直接読み込み
- WindowsのWIC（Windows Imaging Component）APIを利用するすべてのサードパーティ製アプリでのサポート

---

## ✨ 主な機能

- ⚡ **システム全体のサポート** - インストールするだけで、Windowsシェル全体が `.rw2` 形式に対応します。
- 🎨 **高品質なRAW現像** - 信頼性の高い現像ライブラリ `LibRaw` をバックエンドに採用しています。
  - カメラのホワイトバランス（WB）の自動適用
  - 標準sRGB色空間への正しいマッピング
  - AHD（Adaptive Homogeneity-Directed）カラー補間によるディテールの保持
  - 自動明度補正
- 🏷️ **EXIFメタデータのサポート (新機能)** - WIC標準の `IWICMetadataQueryReader` を実装。
  - カメラメーカー、モデル名、シャッタースピード、F値、ISO感度、焦点距離、撮影日時、画像の向きなどの情報をエクスプローラーの「プロパティ」や「詳細ペイン」で直接確認できます。
- 🚀 **極めて軽量な設計**
  - メモリ共有のための `shared_ptr` 参照モデルを採用し、フレームデコード時のRAWデータコピーを排除。
  - 内蔵JPEGプレビューの高速展開（`GetPreview`）に対応し、フォトアプリ等で瞬時に画像を表示。
- 🔒 **堅牢性と高いセキュリティ性**
  - **DLL遅延読み込みとディレクトリ隔離**: `DllMain` でのグローバルなDLL検索パスの変更（`SetDefaultDllDirectories` など）を完全に排除。`explorer.exe` などのホストプロセスを汚染することなく、依存関係（`raw.dll` / `lcms2-2.dll` / `zlib1.dll`）を安全に動的ロードします。
  - **COM境界の例外保護**: LibRawなどの例外がCOMの境界を越えてホストをクラッシュさせるのを防ぐ `try/catch` 例外バリアをすべてのAPIに実装。
  - パナソニック専用シグネチャの完全一致マッチングを行い、他のRAWフォーマット（TIFFやARW, NEF等）の読み込みに干渉しません。

---

## 💻 動作環境

- **OS**: Windows 10 または Windows 11 (64-bit専用)
- **権限**: COMコンポーネント登録のためにインストール時に管理者権限が必要
- **開発ツール (ソースビルド時)**: Visual Studio 2022 / CMake 3.15+ / vcpkg

---

## 📥 インストール方法

### 方法 1：リリースパッケージからインストール（推奨）

1. リリースページから最新のアーカイブをダウンロードします。
2. 任意のフォルダに展開します。
3. `install.bat` を右クリックし、**「管理者として実行」** を選択します。
4. 登録成功のダイアログが表示されます。
5. エクスプローラーを再起動するか、PCを再起動して設定を反映させます。

### 方法 2：ソースコードからのビルド

1. 依存関係のインストール（vcpkg）:
   ```batch
   vcpkg install libraw:x64-windows
   ```
2. ビルドスクリプトの実行:
   リポジトリルートにある `setup_and_build.bat` を実行すると、CMake設定からコンパイルまで自動で行われます。
3. インストール:
   `build/Release` フォルダに移動し、`install.bat` を右クリックして **「管理者として実行」** します。

---

## 🗑️ アンインストール

1. コーデックDLLが置かれているフォルダを開きます。
2. `uninstall.bat` を右クリックし、**「管理者として実行」** を選択します。
3. レジストリおよびファイル関連付けが完全に削除されます。

---

## 🧪 動作確認テスト

インストールされたデコーダーの動作テスト用プログラムが同梱されています：

```batch
TestDecoder.exe C:\path\to\your\photo.rw2
```
これにより、WICデコーダーが正しくロードされ、画像寸法、DPIなどの情報を出力したのち、検証用BMP画像を保存します。

EXIFメタデータのテストには以下を使用します：
```batch
TestExif.exe C:\path\to\your\photo.rw2
```

---

## 📄 ライセンス

- 本プロジェクトのコードは **MIT License** に基づき配布されます。
- バックエンドで使用している **LibRaw** は LGPL 2.1 または CDDL 1.0 ライセンスの下でライセンスされています。

---

## 🙏 クレジットと謝辞

- **LibRaw** - https://www.libraw.org/
- **Windows Imaging Component (WIC)** - Microsoft
