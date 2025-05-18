# mruby API 规范

---

## Input 类

### pressed? 方法

#### 参数

| 名称  | 值                     | 是否可选 | 类型         | 备注 |
| ----- | ---------------------- | -------- | ------------ | ---- |
| part: | :sw1, :sw2, :sw3, :sw4 | 否       | 关键字(符号) |      |

#### 返回值 (bool)

- true: 按钮被按下
- false: 条件不满足

#### 代码示例

```ruby
Input.pressed?(part: :sw1)
```

### released? 方法

#### 参数

| 名称  | 值                     | 是否可选 | 类型         | 备注 |
| ----- | ---------------------- | -------- | ------------ | ---- |
| part: | :sw1, :sw2, :sw3, :sw4 | 否       | 关键字(符号) |      |

#### 返回值 (bool)

- true: 按钮被释放
- false: 条件不满足

#### 代码示例

```ruby
Input.released?(part: :sw1)
```

---

## ADC 类

### update! 方法

#### 参数

无

#### 返回值 (bool)

- true: AD 转换成功
- false: AD 转换失败

#### 代码示例

```ruby
ADC.update!
```

### read 方法

#### 参数 (int)

- 第一个参数: 通道 (0: VDDH, 1: VDD, 2: AIN0)

#### 返回值 (float)

- 电压 (单位: V)

#### 代码示例

```ruby
ADC.update!
puts "VDDH: #{ADC.read(0)} V"
puts "VDD : #{ADC.read(1)} V"
puts "AIN0: #{ADC.read(2)} V"
```

---

## Temperature 类

### value 方法

#### 参数

无

#### 返回值 (float)

- 温度 (单位: 摄氏度, 分辨率: 0.25)

#### 代码示例

```ruby
printf "Temperature: %3.2f degC\n", Temperature.value()
```

---

## LED 类

### set 方法

#### 参数

| 名称   | 值 (**粗体**: 默认) | 是否可选 | 类型         | 备注                 |
| ------ | ------------------- | -------- | ------------ | -------------------- |
| part:  | :led1, :led2, :led3 | 否       | 关键字(符号) | :led3 仅用于系统任务 |
| state: | true, **false**     | 是       | 关键字(bool) |                      |

#### 返回值 (bool)

- true: 成功
- false: 失败

#### 代码示例

```ruby
LED.set(part: :led1, state: true)
```

---

## PWM 类

### set 方法

#### 参数 (int)

- 第一个参数: 频率 (Hz)
- 第二个参数: 占空比 (%)

#### 返回值 (bool)

- true: 成功
- false: 失败

#### 代码示例

```ruby
PWM.set(1_000, 50) # 1kHz, 50%
```

---

## I2C 类

### read 方法

#### 参数

- 第 1 个参数: I2C 地址 (uint16_t)
- 第 2 个参数: 寄存器地址 (uint8_t)
- 第 3 个参数: 大小 (uint8_t)

#### 返回值 (Array)

- Array[0]: 返回值 (0 表示成功，非零表示错误)
- Array[1 及以后]: 实际数据

#### 代码示例

```ruby
tmp = I2C.read(0x5C, 0x00, 4)
printf "I2C read: %d %02X %02X %02X %02X", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]
```

### write 方法

#### 参数

- 第 1 个参数: I2C 地址 (uint16_t)
- 第 2 个参数: 寄存器地址 (uint8_t)
- 第 3 个参数: 数据(Array)

#### 返回值 (int)

- 0: 成功
- 非零: 错误

#### 代码示例

```ruby
tmp = [0xE]
I2C.write(0x76, 0xF5, tmp)
```

---

## BLE 类

### state 方法

#### 参数

无

#### 返回值 (int)

- 0: BLE 关闭状态
- 1: BLE 广播状态
- 2: BLE 已连接状态

#### 代码示例

```ruby
case BLE.state()
  when 0
    # 关闭
  when 1
    # 广播中
  when 2
    # 已连接
end
```

---

## Blink 类

### req_reload? 方法

#### 参数

无

#### 返回值 (bool)

- true: 存在重载请求
- false: 无重载请求

#### 代码示例

```ruby
while true
  return if Blink.req_reload?
  # 主要处理
end
```
