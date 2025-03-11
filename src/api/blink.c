/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.c
 * @brief Implementation of Blink API for mruby/c
 * @details Implements the Blink class and methods for mruby/c scripts
 */
#include "blink.h"

#include "../../mrubyc/src/mrubyc.h"
#include "../app/mrubyc_vm.h"
#include "../lib/fn.h"

/**
 * @brief Forward declaration for reload status getter method
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the Blink class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_blink_define(void) {
  mrb_class *class_blink;
  class_blink = mrbc_define_class(0, "Blink", mrbc_class_object);
  mrbc_define_method(0, class_blink, "req_reload?", c_get_reload);
  return kSuccess;
}

/**
 * @brief Gets the reload status from the mruby/c VM
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc) {
  SET_BOOL_RETURN(app_mrubyc_vm_get_reload());
}
