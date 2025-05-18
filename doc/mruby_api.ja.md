# mruby API 仕様

---

## Input クラス

### pressed? メソッド

#### 引数

| 名前  | 値                     | 省略可能 | 型                   | 備考 |
| ----- | ---------------------- | -------- | -------------------- | ---- |
| part: | :sw1, :sw2, :sw3, :sw4 | 不可     | キーワード(シンボル) |      |

#### 戻り値 (bool)

- true: ボタンが押されている
- false: 条件を満たしていない

#### コード例

```ruby
Input.pressed?(part: :sw1)
```

### released? メソッド

#### 引数

| 名前  | 値                     | 省略可能 | 型                   | 備考 |
| ----- | ---------------------- | -------- | -------------------- | ---- |
| part: | :sw1, :sw2, :sw3, :sw4 | 不可     | キーワード(シンボル) |      |

#### 戻り値 (bool)

- true: ボタンが離されている
- false: 条件を満たしていない

#### コード例

```ruby
Input.released?(part: :sw1)
```

---

## ADC クラス

### update! メソッド

#### 引数

なし

#### 戻り値 (bool)

- true: AD 変換成功
- false: AD 変換失敗

#### コード例

```ruby
ADC.update!
```

### read メソッド

#### 引数 (int)

- 第 1 引数: チャンネル (0: VDDH, 1: VDD, 2: AIN0)

#### 戻り値 (float)

- 電圧 (単位: V)

#### コード例

```ruby
ADC.update!
puts "VDDH: #{ADC.read(0)} V"
puts "VDD : #{ADC.read(1)} V"
puts "AIN0: #{ADC.read(2)} V"
```

---

## Temperature クラス

### value メソッド

#### 引数

なし

#### 戻り値 (float)

- 温度 (単位: 度 C, 解像度: 0.25)

#### コード例

```ruby
printf "Temperature: %3.2f degC\n", Temperature.value()
```

---

## LED クラス

### set メソッド

#### 引数

| 名前   | 値 (**太字**: デフォルト) | 省略可能 | 型                   | 備考                       |
| ------ | ------------------------- | -------- | -------------------- | -------------------------- |
| part:  | :led1, :led2, :led3       | 不可     | キーワード(シンボル) | :led3 はシステムタスク専用 |
| state: | true, **false**           | 可能     | キーワード(bool)     |                            |

#### 戻り値 (bool)

- true: 成功
- false: 失敗

#### コード例

```ruby
LED.set(part: :led1, state: true)
```

---

## PWM クラス

### set メソッド

#### 引数 (int)

- 第 1 引数: 周波数 (Hz)
- 第 2 引数: デューティサイクル (%)

#### 戻り値 (bool)

- true: 成功
- false: 失敗

#### コード例

```ruby
PWM.set(1_000, 50) # 1kHz, 50%
```

---

## I2C クラス

### read メソッド

#### 引数

- 第 1 引数: I2C アドレス (uint16_t)
- 第 2 引数: レジスタアドレス (uint8_t)
- 第 3 引数: サイズ (uint8_t)

#### 戻り値 (Array)

- Array[0]: 戻り値 (0 は成功、0 以外はエラー)
- Array[1 以降]: 実際のデータ

#### コード例

```ruby
tmp = I2C.read(0x5C, 0x00, 4)
printf "I2C read: %d %02X %02X %02X %02X", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]
```

### write メソッド

#### 引数

- 第 1 引数: I2C アドレス (uint16_t)
- 第 2 引数: レジスタアドレス (uint8_t)
- 第 3 引数: データ(Array)

#### 戻り値 (int)

- 0: 成功
- 0 以外: エラー

#### コード例

```ruby
tmp = [0xE]
I2C.write(0x76, 0xF5, tmp)
```

---

## BLE クラス

### state メソッド

#### 引数

なし

#### 戻り値 (int)

- 0: BLE オフ状態
- 1: BLE アドバタイジング状態
- 2: BLE 接続状態

#### コード例

```ruby
case BLE.state()
  when 0
    # オフ
  when 1
    # アドバタイジング
  when 2
    # 接続済み
end
```

---

## Blink クラス

### req_reload? メソッド

#### 引数

なし

#### 戻り値 (bool)

- true: リロード要求が存在する
- false: リロード要求なし

#### コード例

```ruby
while true
  return if Blink.req_reload?
  # メイン処理
end
```
