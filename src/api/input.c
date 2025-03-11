/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file input.c
 * @brief Implementation of Input API for mruby/c
 * @details Implements the Input class and methods for mruby/c scripts to handle
 * button inputs
 */
#include "input.h"

#include <stdint.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/gpio.h"
#include "../lib/fn.h"
#include "api.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_input, LOG_LEVEL_DBG);

/**
 * @brief Converts a symbol ID to the corresponding GPIO enum
 *
 * @param kSymbol The symbol ID to convert
 * @return drv_gpio_t The corresponding GPIO enum
 */
static drv_gpio_t sym_to_gpio(const int16_t kSymbol);

/**
 * @brief Forward declarations for button state methods
 */
static void c_get_sw_pressed(mrb_vm *vm, mrb_value *v, int argc);
static void c_get_sw_released(mrb_vm *vm, mrb_value *v, int argc);
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the Input class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_input_define(void) {
  mrb_class *class_input;
  class_input = mrbc_define_class(0, "Input", mrbc_class_object);
  mrbc_define_method(0, class_input, "pressed?", c_get_sw_pressed);
  mrbc_define_method(0, class_input, "released?", c_get_sw_released);
  if (kSuccess == api_api_get_allow_expert_api()) {
    mrbc_define_method(0, class_input, "sw1_read", c_sw1_read);
  }
  return kSuccess;
}

/**
 * @brief Checks if a button is currently pressed
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
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

/**
 * @brief Checks if a button is currently released
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
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
// c_sw1_read
static void c_sw1_read(mrb_vm *vm, mrb_value *v, int argc) {
  SET_INT_RETURN(1);  // DUMMY
}

/**
 * @brief Converts a symbol ID to the corresponding GPIO enum
 *
 * @param kSymbol The symbol ID to convert
 * @return drv_gpio_t The corresponding GPIO enum
 */
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
