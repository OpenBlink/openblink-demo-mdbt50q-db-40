/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "pwm.h"

#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../drv/pwm.h"
#include "../lib/fn.h"
#include "api.h"
#include "symbol.h"

LOG_MODULE_REGISTER(api_pwm, LOG_LEVEL_DBG);

// **************************************************************************
// forward declarations
static void c_set_pwm(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_pwm_define
fn_t api_pwm_define(void) {
  mrb_class *class_pwm;
  class_pwm = mrbc_define_class(0, "PWM", mrbc_class_object);
  mrbc_define_method(0, class_pwm, "set", c_set_pwm);
  return kSuccess;
}

// **************************************************************************
// c_set_pwm
static void c_set_pwm(mrb_vm *vm, mrb_value *v, int argc) {
  SET_FALSE_RETURN();
  if ((true == MRBC_ISNUMERIC(v[1])) && (true == MRBC_ISNUMERIC(v[2]))) {
    if (kSuccess ==
        drv_pwm_set((uint32_t)GET_INT_ARG(1), (uint8_t)GET_INT_ARG(2))) {
      SET_TRUE_RETURN();
    }
  }
}
