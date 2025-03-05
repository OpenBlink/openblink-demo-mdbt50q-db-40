/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "input.h"

#include <stdint.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/gpio.h"
#include "../lib/fn.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_input, LOG_LEVEL_DBG);

static drv_gpio_t sym_to_gpio(const int16_t kSymbol);

// **************************************************************************
// forward declaration
static void c_get_sw_pressed(mrb_vm *vm, mrb_value *v, int argc);
static void c_get_sw_released(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_input_define
fn_t api_input_define(void) {
  mrb_class *class_input;
  class_input = mrbc_define_class(0, "Input", mrbc_class_object);
  mrbc_define_method(0, class_input, "pressed?", c_get_sw_pressed);
  mrbc_define_method(0, class_input, "released?", c_get_sw_released);
  return kSuccess;
}

// **************************************************************************
// c_get_sw_pressed
static void c_get_sw_pressed(mrb_vm *vm, mrb_value *v, int argc) {
  int16_t tgt = -1;
  SET_FALSE_RETURN();

  // ==============================
  MRBC_KW_ARG(part);
  do {
    if (!MRBC_KW_MANDATORY(part)) break;
    if (!MRBC_KW_END()) break;

    if (MRBC_TT_SYMBOL == part.tt) {
      tgt = (int16_t)part.i;
    } else {
      break;
    }

  } while (0);
  MRBC_KW_DELETE(part);
  // ==============================

  if (true == drv_gpio_get(sym_to_gpio(tgt))) {
    SET_TRUE_RETURN();
  }
}

// **************************************************************************
// c_get_sw_released
static void c_get_sw_released(mrb_vm *vm, mrb_value *v, int argc) {
  int16_t tgt = -1;
  SET_FALSE_RETURN();

  // ==============================
  MRBC_KW_ARG(part);
  do {
    if (!MRBC_KW_MANDATORY(part)) break;
    if (!MRBC_KW_END()) break;

    if (MRBC_TT_SYMBOL == part.tt) {
      tgt = (int16_t)part.i;
    } else {
      break;
    }

  } while (0);
  MRBC_KW_DELETE(part);
  // ==============================

  if (false == drv_gpio_get(sym_to_gpio(tgt))) {
    SET_TRUE_RETURN();
  }
}

// **************************************************************************
// sym_to_gpio
static drv_gpio_t sym_to_gpio(const int16_t kSymbol) {
  if (api_symbol_get_id(kSymbolSW1) == kSymbol) {
    return kDrvGpioSW1;
  } else if (api_symbol_get_id(kSymbolSW2) == kSymbol) {
    return kDrvGpioSW2;
  } else if (api_symbol_get_id(kSymbolSW3) == kSymbol) {
    return kDrvGpioSW3;
  } else if (api_symbol_get_id(kSymbolSW4) == kSymbol) {
    return kDrvGpioSW4;
  } else {
    return kDrvGpioSW1;
  }
}
