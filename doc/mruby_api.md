# mruby API Specification

---

## Input Class

### pressed? Method

#### Arguments

| Name  | Values                 | Optional | Type            | Notes |
| ----- | ---------------------- | -------- | --------------- | ----- |
| part: | :sw1, :sw2, :sw3, :sw4 | No       | Keyword(Symbol) |       |

#### Return Value (bool)

- true: Button is pressed
- false: Condition not met

#### Code Example

```ruby
Input.pressed?(part: :sw1)
```

### released? Method

#### Arguments

| Name  | Values                 | Optional | Type            | Notes |
| ----- | ---------------------- | -------- | --------------- | ----- |
| part: | :sw1, :sw2, :sw3, :sw4 | No       | Keyword(Symbol) |       |

#### Return Value (bool)

- true: Button is released
- false: Condition not met

#### Code Example

```ruby
Input.released?(part: :sw1)
```

---

## ADC Class

### update! Method

#### Arguments

None

#### Return Value (bool)

- true: AD conversion successful
- false: AD conversion failed

#### Code Example

```ruby
ADC.update!
```

### read Method

#### Arguments (int)

- First argument: Channel (0: VDDH, 1: VDD, 2: AIN0)

#### Return Value (float)

- Voltage (unit: V)

#### Code Example

```ruby
ADC.update!
puts "VDDH: #{ADC.read(0)} V"
puts "VDD : #{ADC.read(1)} V"
puts "AIN0: #{ADC.read(2)} V"
```

---

## Temperature Class

### value Method

#### Arguments

None

#### Return Value (float)

- Temperature (unit: degC, resolution: 0.25)

#### Code Example

```ruby
printf "Temperature: %3.2f degC\n", Temperature.value()
```

---

## LED Class

### set Method

#### Arguments

| Name   | Values (**bold**: default) | Optional | Type            | Notes                          |
| ------ | -------------------------- | -------- | --------------- | ------------------------------ |
| part:  | :led1, :led2, :led3        | No       | Keyword(Symbol) | :led3 is for system tasks only |
| state: | true, **false**            | Yes      | Keyword(bool)   |                                |

#### Return Value (bool)

- true: Success
- false: Failure

#### Code Example

```ruby
LED.set(part: :led1, state: true)
```

---

## PWM Class

### set Method

#### Arguments (int)

- First argument: Frequency (Hz)
- Second argument: Duty cycle (%)

#### Return Value (bool)

- true: Success
- false: Failure

#### Code Example

```ruby
PWM.set(1_000, 50) # 1kHz, 50%
```

---

## I2C Class

### read Method

#### Arguments

- 1st argument: I2C address (uint16_t)
- 2nd argument: Register address (uint8_t)
- 3rd argument: Size (uint8_t)

#### Return Value (Array)

- Array[0]: Return value (0 indicates success, non-zero indicates error)
- Array[1 and beyond]: Actual data

#### Code Example

```ruby
tmp = I2C.read(0x5C, 0x00, 4)
printf "I2C read: %d %02X %02X %02X %02X", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]
```

### write Method

#### Arguments

- 1st argument: I2C address (uint16_t)
- 2nd argument: Register address (uint8_t)
- 3rd argument: Data(Array)

#### Return Value (int)

- 0: Success
- Non-zero: Error

#### Code Example

```ruby
tmp = [0xE]
I2C.write(0x76, 0xF5, tmp)
```

---

## BLE Class

### state Method

#### Arguments

None

#### Return Value (int)

- 0: BLE Off state
- 1: BLE Advertising state
- 2: BLE Connected state

#### Code Example

```ruby
case BLE.state()
  when 0
    # Off
  when 1
    # Advertising
  when 2
    # Connected
end
```

---

## Blink Class

### req_reload? Method

#### Arguments

None

#### Return Value (bool)

- true: Reload request exists
- false: No reload request

#### Code Example

```ruby
while true
  return if Blink.req_reload?
  # Main processing
end
```
