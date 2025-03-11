# OpenBlink Bluetooth Communication Specification

English text is followed by Japanese translation. / 英語の文章の後に日本語訳が続きます。

## Overview

This document describes the Bluetooth Low Energy (BLE) communication specification for the OpenBlink system. It includes service and characteristic UUIDs, protocol details, data structures, and communication flow.

## Service and Characteristics

### OpenBlink Service

- **Service UUID**: `227da52c-e13a-412b-befb-ba2256bb7fbe`
- **Description**: Primary service for OpenBlink device communication

### Characteristics

| Characteristic | UUID                                   | Properties                    | Description                              |
| -------------- | -------------------------------------- | ----------------------------- | ---------------------------------------- |
| Program        | `ad9fdd56-1135-4a84-923c-ce5a244385e7` | Write, Write Without Response | Used for bytecode transfer and execution |
| Console        | `a015b3de-185a-4252-aa04-7a87d38ce148` | Notify                        | Used for debug output and notifications  |
| Status         | `ca141151-3113-448b-b21a-6a6203d253ff` | Read                          | Provides device status information       |

## Protocol

### Blink Protocol

- **Version**: 0x01
- **Description**: Protocol for transferring and executing bytecode on the OpenBlink device

### Command Types

| Command | Code | Description                       |
| ------- | ---- | --------------------------------- |
| Data    | 'D'  | Transfers a chunk of bytecode     |
| Program | 'P'  | Executes the transferred bytecode |
| Reset   | 'R'  | Resets the device                 |
| Reload  | 'L'  | Reloads the bytecode              |

## Data Structures

### BLINK_CHUNK_HEADER

- **Size**: 2 bytes
- **Description**: Common header for all Blink protocol commands

| Field   | Type    | Size   | Description                          |
| ------- | ------- | ------ | ------------------------------------ |
| version | uint8_t | 1 byte | Blink protocol version (0x01)        |
| command | uint8_t | 1 byte | Command type ('D', 'P', 'R', or 'L') |

### BLINK_CHUNK_DATA

- **Size**: 6 bytes + data
- **Description**: Structure for data chunk transfers

| Field  | Type               | Size     | Description               |
| ------ | ------------------ | -------- | ------------------------- |
| header | BLINK_CHUNK_HEADER | 2 bytes  | Common header             |
| offset | uint16_t           | 2 bytes  | Offset in bytecode buffer |
| size   | uint16_t           | 2 bytes  | Size of data chunk        |
| data   | uint8_t[]          | Variable | Actual bytecode data      |

### BLINK_CHUNK_PROGRAM

- **Size**: 8 bytes
- **Description**: Structure for program execution command

| Field    | Type               | Size    | Description              |
| -------- | ------------------ | ------- | ------------------------ |
| header   | BLINK_CHUNK_HEADER | 2 bytes | Common header            |
| length   | uint16_t           | 2 bytes | Total bytecode length    |
| crc      | uint16_t           | 2 bytes | CRC16 checksum           |
| slot     | uint8_t            | 1 byte  | Target slot for bytecode |
| reserved | uint8_t            | 1 byte  | Reserved for future use  |

## Communication Flow

### Bytecode Transfer and Execution

```
Client                                      OpenBlink Device
  |                                               |
  |--- Discover OpenBlink Service --------------->|
  |<-- Service and Characteristics Found ---------|
  |                                               |
  |--- Write Data Chunk 1 to Program Char ------->|
  |--- Write Data Chunk 2 to Program Char ------->|
  |--- Write Data Chunk n to Program Char ------->|
  |                                               |
  |--- Write Program Command to Program Char ---->|
  |                   (CRC check)                 |
  |                                               |
  |--- Write Reset/Reload Command --------------->|
  |                                               |
```

### BLE Events

| Event                  | Description                    |
| ---------------------- | ------------------------------ |
| BLE_EVENT_INITIALIZED  | BLE stack has been initialized |
| BLE_EVENT_CONNECTED    | BLE connection established     |
| BLE_EVENT_DISCONNECTED | BLE connection terminated      |
| BLE_EVENT_RECEIVED     | Data received over BLE         |
| BLE_EVENT_SENT         | Data sent over BLE             |
| BLE_EVENT_BLINK        | Blink bytecode received        |
| BLE_EVENT_STATUS       | Status information requested   |
| BLE_EVENT_REBOOT       | Reboot request received        |
| BLE_EVENT_RELOAD       | Reload request received        |

## Error Handling

### Error Notifications

Errors during bytecode transfer or execution are reported through notifications on the Console characteristic. The notification contains an error message string.

### Common Errors

| Error                               | Description                                  |
| ----------------------------------- | -------------------------------------------- |
| "ERROR: Blink version mismatch"     | Protocol version is not supported            |
| "ERROR: Blink data size error"      | Data chunk size does not match expected size |
| "ERROR: Size exceeds buffer limits" | Bytecode size exceeds maximum allowed size   |
| "ERROR: CRC mismatch"               | CRC checksum verification failed             |
| "ERROR: Blink program error"        | Error during bytecode execution              |
| "ERROR: Blink unknown type"         | Unknown command type received                |

## Implementation Notes

### Maximum Bytecode Size

The maximum bytecode size is defined by `BLINK_MAX_BYTECODE_SIZE` in the implementation.

### CRC Calculation

CRC16 checksum is calculated using the `crc16_reflect` function with the following parameters:

- Initial value: 0xd175
- Polynomial: 0xFFFF
- Input: bytecode buffer
- Length: bytecode length

---

# OpenBlink Bluetooth 通信仕様

## 概要

このドキュメントは OpenBlink システムの Bluetooth Low Energy（BLE）通信仕様を説明します。サービスと特性の UUID、プロトコルの詳細、データ構造、通信フローが含まれています。

## サービスと特性

### OpenBlink サービス

- **サービス UUID**: `227da52c-e13a-412b-befb-ba2256bb7fbe`
- **説明**: OpenBlink デバイス通信のためのプライマリサービス

### 特性

| 特性       | UUID                                   | プロパティ                 | 説明                         |
| ---------- | -------------------------------------- | -------------------------- | ---------------------------- |
| プログラム | `ad9fdd56-1135-4a84-923c-ce5a244385e7` | 書き込み、応答なし書き込み | バイトコード転送と実行に使用 |
| コンソール | `a015b3de-185a-4252-aa04-7a87d38ce148` | 通知                       | デバッグ出力と通知に使用     |
| ステータス | `ca141151-3113-448b-b21a-6a6203d253ff` | 読み取り                   | デバイスステータス情報を提供 |

## プロトコル

### Blink プロトコル

- **バージョン**: 0x01
- **説明**: OpenBlink デバイスでバイトコードを転送および実行するためのプロトコル

### コマンドタイプ

| コマンド   | コード | 説明                         |
| ---------- | ------ | ---------------------------- |
| データ     | 'D'    | バイトコードのチャンクを転送 |
| プログラム | 'P'    | 転送されたバイトコードを実行 |
| リセット   | 'R'    | デバイスをリセット           |
| リロード   | 'L'    | バイトコードをリロード       |

## データ構造

### BLINK_CHUNK_HEADER

- **サイズ**: 2 バイト
- **説明**: すべての Blink プロトコルコマンドの共通ヘッダー

| フィールド | 型      | サイズ   | 説明                                       |
| ---------- | ------- | -------- | ------------------------------------------ |
| version    | uint8_t | 1 バイト | Blink プロトコルバージョン（0x01）         |
| command    | uint8_t | 1 バイト | コマンドタイプ（'D'、'P'、'R'、または'L'） |

### BLINK_CHUNK_DATA

- **サイズ**: 6 バイト + データ
- **説明**: データチャンク転送のための構造体

| フィールド | 型                 | サイズ   | 説明                               |
| ---------- | ------------------ | -------- | ---------------------------------- |
| header     | BLINK_CHUNK_HEADER | 2 バイト | 共通ヘッダー                       |
| offset     | uint16_t           | 2 バイト | バイトコードバッファ内のオフセット |
| size       | uint16_t           | 2 バイト | データチャンクのサイズ             |
| data       | uint8_t[]          | 可変     | 実際のバイトコードデータ           |

### BLINK_CHUNK_PROGRAM

- **サイズ**: 8 バイト
- **説明**: プログラム実行コマンドのための構造体

| フィールド | 型                 | サイズ   | 説明                             |
| ---------- | ------------------ | -------- | -------------------------------- |
| header     | BLINK_CHUNK_HEADER | 2 バイト | 共通ヘッダー                     |
| length     | uint16_t           | 2 バイト | バイトコードの総長               |
| crc        | uint16_t           | 2 バイト | CRC16 チェックサム               |
| slot       | uint8_t            | 1 バイト | バイトコードのターゲットスロット |
| reserved   | uint8_t            | 1 バイト | 将来の使用のために予約           |

## 通信フロー

### バイトコード転送と実行

```
クライアント                                OpenBlinkデバイス
  |                                               |
  |--- OpenBlinkサービスの発見 ------------------>|
  |<-- サービスと特性の発見 ----------------------|
  |                                               |
  |--- データチャンク1をプログラム特性に書き込み ->|
  |--- データチャンク2をプログラム特性に書き込み ->|
  |--- データチャンクnをプログラム特性に書き込み ->|
  |                                               |
  |--- プログラムコマンドをプログラム特性に書き込み >|
  |                   (CRCチェック)               |
  |                                               |
  |--- リセット/リロードコマンドの書き込み ------->|
  |                                               |
```

### BLE イベント

| イベント               | 説明                         |
| ---------------------- | ---------------------------- |
| BLE_EVENT_INITIALIZED  | BLE スタックが初期化された   |
| BLE_EVENT_CONNECTED    | BLE 接続が確立された         |
| BLE_EVENT_DISCONNECTED | BLE 接続が終了した           |
| BLE_EVENT_RECEIVED     | BLE 経由でデータを受信した   |
| BLE_EVENT_SENT         | BLE 経由でデータを送信した   |
| BLE_EVENT_BLINK        | Blink バイトコードを受信した |
| BLE_EVENT_STATUS       | ステータス情報が要求された   |
| BLE_EVENT_REBOOT       | 再起動要求を受信した         |
| BLE_EVENT_RELOAD       | リロード要求を受信した       |

## エラー処理

### エラー通知

バイトコード転送または実行中のエラーは、コンソール特性を通じて通知されます。通知にはエラーメッセージ文字列が含まれます。

### 一般的なエラー

| エラー                              | 説明                                           |
| ----------------------------------- | ---------------------------------------------- |
| "ERROR: Blink version mismatch"     | プロトコルバージョンがサポートされていない     |
| "ERROR: Blink data size error"      | データチャンクサイズが予想サイズと一致しない   |
| "ERROR: Size exceeds buffer limits" | バイトコードサイズが許容最大サイズを超えている |
| "ERROR: CRC mismatch"               | CRC チェックサム検証に失敗した                 |
| "ERROR: Blink program error"        | バイトコード実行中のエラー                     |
| "ERROR: Blink unknown type"         | 不明なコマンドタイプを受信した                 |

## 実装に関する注意

### 最大バイトコードサイズ

最大バイトコードサイズは実装内で`BLINK_MAX_BYTECODE_SIZE`として定義されています。

### CRC 計算

CRC16 チェックサムは以下のパラメータを使用して`crc16_reflect`関数で計算されます：

- 初期値：0xd175
- 多項式：0xFFFF
- 入力：バイトコードバッファ
- 長さ：バイトコード長
