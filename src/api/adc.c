/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "adc.h"

#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/adc.h"
#include "../lib/fn.h"
#include "api.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_adc, LOG_LEVEL_DBG);

// **************************************************************************
// forward declaration
static void c_update_adc(mrb_vm *vm, mrb_value *v, int argc);
static void c_get_adc(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_adc_define
fn_t api_adc_define(void) {
  mrb_class *class_adc;
  class_adc = mrbc_define_class(0, "ADC", mrbc_class_object);
  mrbc_define_method(0, class_adc, "update!", c_update_adc);
  mrbc_define_method(0, class_adc, "read", c_get_adc);
  return kSuccess;
}

// **************************************************************************
// c_update_adc
static void c_update_adc(mrb_vm *vm, mrb_value *v, int argc) {
  if (kSuccess == drv_adc_update()) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

// **************************************************************************
// c_get_adc
static void c_get_adc(mrb_vm *vm, mrb_value *v, int argc) {
  SET_FLOAT_RETURN(0.0f);
  if (true == MRBC_ISNUMERIC(v[1])) {
    SET_FLOAT_RETURN((float)(drv_adc_get(GET_INT_ARG(1))) / 1000);
  }
}
