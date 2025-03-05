/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "led.h"

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/gpio.h"
#include "../lib/fn.h"
#include "api.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_led, LOG_LEVEL_DBG);

// **************************************************************************
// forward declarations
static void c_set_led(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_led_define
fn_t api_led_define(void) {
  mrb_class *class_led;
  class_led = mrbc_define_class(0, "LED", mrbc_class_object);
  mrbc_define_method(0, class_led, "set", c_set_led);
  return kSuccess;
}

// **************************************************************************
// c_set_led
static void c_set_led(mrb_vm *vm, mrb_value *v, int argc) {
  int16_t tgt = -1;
  bool req = false;
  SET_FALSE_RETURN();

  // ==============================
  MRBC_KW_ARG(part, state);
  do {
    if (!MRBC_KW_MANDATORY(part)) break;
    if (!MRBC_KW_END()) break;

    if (MRBC_TT_SYMBOL == part.tt) {
      tgt = (int16_t)part.i;
    } else {
      break;
    }

    if (MRBC_KW_ISVALID(state)) {
      req = api_api_get_bool(state.tt);
    }

  } while (0);
  MRBC_KW_DELETE(part, state);
  // ==============================

  // LED1
  if (api_symbol_get_id(kSymbolLED1) == tgt) {
    drv_gpio_set(kDrvGpioLED1, req);
    SET_TRUE_RETURN();
  }
  // LED2
  if (api_symbol_get_id(kSymbolLED2) == tgt) {
    drv_gpio_set(kDrvGpioLED2, req);
    SET_TRUE_RETURN();
  }
  // LED3
  if (api_symbol_get_id(kSymbolLED3) == tgt) {
    drv_gpio_set(kDrvGpioLED3, req);
    SET_TRUE_RETURN();
  }
}
