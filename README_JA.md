# RW2 WIC Codec

WindowsでPanasonic RW2をネイティブに扱うためのコーデック

[![CI](https://github.com/magnum-qin/RW2-WIC-Codec/actions/workflows/ci.yml/badge.svg)](https://github.com/magnum-qin/RW2-WIC-Codec/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/magnum-qin/RW2-WIC-Codec)](https://github.com/magnum-qin/RW2-WIC-Codec/releases/latest)
[![Downloads](https://img.shields.io/github/downloads/magnum-qin/RW2-WIC-Codec/total)](https://github.com/magnum-qin/RW2-WIC-Codec/releases)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11%20x64-0078D4)
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C)

[English](README.md) · [简体中文](README_CN.md) · **日本語**

RW2 WIC Codecは、64ビットWindows向けのWindows Imaging Componentデコーダーです。インストールすると、Windowsエクスプローラー、Windowsフォト、その他のWIC対応アプリケーションから、対応するPanasonic `.rw2` RAWファイルをデコードできます。

[最新版をダウンロード](https://github.com/magnum-qin/RW2-WIC-Codec/releases/latest) · [不具合を報告](https://github.com/magnum-qin/RW2-WIC-Codec/issues/new?template=bug_report.yml) · [トラブルシューティング](TROUBLESHOOTING.md)

## 主な機能

- Panasonic RW2向けのシステム全体のWICデコード
- エクスプローラーのサムネイルとプレビュー
- WindowsフォトおよびWIC対応アプリケーション
- LibRawによるカメラホワイトバランス、sRGB出力、PPGデモザイク
- `IWICBitmapDecoder::GetPreview`による埋め込みJPEGプレビュー
- `IWICMetadataQueryReader`によるカメラ、露出、ISO、焦点距離、撮影日時、向き、寸法などのEXIF情報
- デコーダーとフレーム間の不要なコピーを抑える共有RAWバッファー
- Codecと同じディレクトリからのスレッドセーフな依存DLL読み込み
- 破損入力の影響を抑えるCOM境界の例外バリア

```mermaid
flowchart LR
    A["Panasonic RW2"] --> B["Windows WIC"]
    B --> C["RW2 WIC Codec"]
    C --> D["LibRaw"]
    D --> E["24-bit BGR / sRGB"]
    E --> F["Explorer、Photos、WICアプリ"]
```

## 動作環境と互換性

| 項目 | 要件 |
| --- | --- |
| OS | Windows 10またはWindows 11 |
| アーキテクチャ | x64のみ |
| インストール | 管理者権限が必要 |
| ソースビルド | Visual Studio 2022、CMake 3.15+、vcpkg |
| デコードバックエンド | LibRaw |

対応状況は、リリースに含まれるLibRawのバージョンとカメラ／ファームウェアの組み合わせに依存します。読み込めない場合は、カメラ型番と関連するエラー出力を不具合報告に含めてください。

## インストール

### インストーラー

1. [最新リリース](https://github.com/magnum-qin/RW2-WIC-Codec/releases/latest)を開きます。
2. `RW2Codec_Setup_v*.exe`をダウンロードします。
3. インストーラーを実行し、管理者権限を許可します。
4. サムネイルが更新されない場合は、エクスプローラーまたはWindowsを再起動します。

現在のリリースはコード署名されていないため、Windows SmartScreenの警告が表示される場合があります。本リポジトリのReleasesページから取得し、実行前にGitHubが表示するSHA-256ダイジェストを確認してください。

### ポータブルパッケージ

`RW2Codec-v*-x64.zip`を永続的なフォルダーへ展開し、`install.bat`を管理者として実行します。登録後はCodecと依存DLLを別々に移動しないでください。

### アンインストール

インストーラー版はWindowsの「インストールされているアプリ」から削除します。ポータブル版は、フォルダーを削除する前に`uninstall.bat`を管理者として実行します。

## ソースからビルド

Visual Studio 2022の「C++によるデスクトップ開発」、CMake、vcpkgをインストールしてから実行します。

```batch
vcpkg install --triplet x64-windows
cmake -S . -B build -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

対話型セットアップには`setup_and_build.bat`も利用できます。詳細な中国語ガイドは[BUILD_GUIDE_CN.md](BUILD_GUIDE_CN.md)です。

## RW2ファイルでの確認

自動CTestは診断用実行ファイルの起動を確認します。完全なデコードは、登録済みCodecと実際のRW2サンプルを必要とする統合テストです。

```batch
TestDecoder.exe C:\Photos\sample.rw2
TestExif.exe C:\Photos\sample.rw2
TestPerf.exe C:\Photos\sample.rw2
```

- `TestDecoder`はWIC経由でデコードし、目視確認用BMPを書き出します。
- `TestExif`はWICから取得した一部のEXIF値を表示します。
- `TestPerf`はLibRawの補間モードを比較するローカル診断であり、公開ベンチマークではありません。

RW2サンプルを公開する前に、位置情報、カメラのシリアル番号、著者情報などの機密メタデータを確認してください。

## リポジトリ構成

```text
.
├── .github/                 # CI、リリース、コミュニティテンプレート
├── include/                 # COMおよびWIC宣言
├── src/                     # Codec、登録、メタデータ、DLLローダー
├── tests/                   # 診断およびスモークテスト
├── scripts/                 # ポータブル版の登録／解除スクリプト
├── CMakeLists.txt           # ビルドとCTest設定
├── vcpkg.json               # LibRaw依存マニフェスト
└── RW2Codec_Setup.iss       # Inno Setup定義
```

詳細は[PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)を参照してください。

## コントリビューションとライセンス

不具合報告、カメラ互換性情報、文書修正、目的の明確なPull Requestを歓迎します。[CONTRIBUTING.md](CONTRIBUTING.md)と[行動規範](CODE_OF_CONDUCT.md)を確認してください。

ソースコードは[MIT License](LICENSE)です。LibRawはLGPL 2.1またはCDDL 1.0で配布されています。バイナリを再配布する前に[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)を参照してください。
