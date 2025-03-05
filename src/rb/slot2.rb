led = true
sw2_last = Input.pressed?(part: :sw2)

puts "Hello World!"

while true
  return if Blink.req_reload?

  led = !led
  led = true if Input.pressed?(part: :sw1)
  led = false if Input.pressed?(part: :sw3)
  LED.set(part: :led1, state: led)

  sw2 = Input.pressed?(part: :sw2)
  if sw2_last != sw2
    sw2_last = sw2
    puts (sw2 ? "SW2 pressed" : "SW2 released")
  end

  sleep_ms 500

end
