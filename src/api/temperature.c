/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file temperature.c
 * @brief Implementation of Temperature API for mruby/c
 * @details Implements the Temperature class and methods for mruby/c scripts
 */
#include "temperature.h"

#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/hal/die_temperature.h"
#include "../lib/fn.h"
#include "api.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_temperature, LOG_LEVEL_DBG);

/**
 * @brief Forward declaration for temperature methods
 */
static void c_get_temp(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the Temperature class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_temperature_define(void) {
  mrb_class *class_temp;
  class_temp = mrbc_define_class(0, "Temperature", mrbc_class_object);
  mrbc_define_method(0, class_temp, "value", c_get_temp);
  return kSuccess;
}

/**
 * @brief Gets the current die temperature
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_temp(mrb_vm *vm, mrb_value *v, int argc) {
  SET_FLOAT_RETURN(hal_die_temperature_get());
}
