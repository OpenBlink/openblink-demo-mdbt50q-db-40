=begin
  SPDX-License-Identifier: BSD-3-Clause
  SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
=end
printf "#{RUBY_ENGINE} #{MRUBYC_VERSION} (mruby:#{MRUBY_VERSION} ruby:#{RUBY_VERSION})\n"

while true
  return if Blink.req_reload?
  # BLE状態を表示
  case BLE.state()
  when 0 # Off
    LED.set(part: :led3)
    sleep_ms 100
  when 1 # Advertising
    LED.set(part: :led3, state: true)
    sleep_ms 500
    LED.set(state: false, part: :led3)
    sleep_ms 500
  when 2 # Connected
    LED.set(part: :led3, state: true)
    sleep_ms 100
  end

end
