# OpenBlink Demo MDBT50Q-DB-40

[![English](https://img.shields.io/badge/language-English-blue.svg)](README.md)
[![中文](https://img.shields.io/badge/language-中文-red.svg)](README.zh-CN.md)
[![日本語](https://img.shields.io/badge/language-日本語-green.svg)](README.ja.md)
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/OpenBlink/openblink-demo-mdbt50q-db-40)

## OpenBlink とは

**_OpenBlink_** は **_ViXion Blink_** からフォークされたオープンソースプロジェクトです。

- 高い生産性を持つ軽量言語 Ruby を使用して組み込みデバイスを開発できます。
- プログラムの書き換えとデバッグコンソールは完全にワイヤレスです。（BluetoothLE）
- 書き換え時間は 0.1 秒未満で、マイクロプロセッサの再起動を必要としません。（これを「Blink」と呼んでいます）

## OpenBlink の入手方法

リポジトリをクローンしてサブモジュールを初期化するには、以下のコマンドを実行してください：

```console
$ git clone https://github.com/OpenBlink/openblink-demo-mdbt50q-db-40.git
$ git submodule init
$ git submodule update
```

## 検証済みハードウェア

以下のハードウェアプラットフォームが OpenBlink でテスト済みです：

- Raytac MDBT50Q-DB-40 (Board target: raytac_mdbt50q_db_40/nrf52840)

## 開発環境バージョン

- nRF Connect SDK toolchain v2.9.0
- nRF Connect SDK v2.9.0
