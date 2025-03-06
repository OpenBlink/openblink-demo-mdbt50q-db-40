sleep_ms 1
unless ($TASK1.nil?)
  while true
    return if Blink.req_reload?
    $TASK1.call()
  end
end
