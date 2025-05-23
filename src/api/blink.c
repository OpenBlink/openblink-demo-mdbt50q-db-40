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
static bool watchdog_enable[MAX_VM_COUNT];
static bool watchdog_feed[MAX_VM_COUNT];

/**
 * @brief Forward declaration for reload status getter method
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Sample method for string handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_string(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Sample method for array handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Alternative sample method for array handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array2(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Sample method for creating and returning arrays
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array3(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the Blink class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_blink_define(void) {
  mrb_class *class_blink;
  class_blink = mrbc_define_class(0, "Blink", mrbc_class_object);
  mrbc_define_method(0, class_blink, "req_reload?", c_get_reload);
  mrbc_define_method(0, class_blink, "sample_string", c_sample_string);
  mrbc_define_method(0, class_blink, "sample_array", c_sample_array);
  mrbc_define_method(0, class_blink, "sample_array2", c_sample_array2);
  mrbc_define_method(0, class_blink, "sample_array3", c_sample_array3);
  return kSuccess;
}

/**
 * @brief Sample method for string handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_string(mrb_vm *vm, mrb_value *v, int argc) {
  char tmp_char[255];
  size_t tmp_size = 0;

  if (MRBC_TT_STRING == v[1].tt) {
    uint8_t *string = v[1].string->data;
    for (size_t i = 0; i < sizeof(tmp_char) / sizeof(tmp_char[0]); i++) {
      if ((i == v[1].string->size) || ('\0' == string[i])) {
        tmp_size = i;
        break;
      }
      tmp_char[i] = (char)string[i];
    }
  }
}

/**
 * @brief Sample method for array handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array(mrb_vm *vm, mrb_value *v, int argc) {
  uint8_t array[3] = {0};
  if (MRBC_TT_ARRAY == v[1].tt) {
    if (3 != v[1].array->n_stored) return;
    for (size_t i = 0; i < 3; i++) {
      array[i] = (uint8_t)v[1].array->data[i].i;
    }
  }
}

/**
 * @brief Alternative sample method for array handling
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array2(mrb_vm *vm, mrb_value *v, int argc) {
  uint8_t c_array[3] = {0};
  if (MRBC_TT_ARRAY == v[1].tt) {
    if (3 == v[1].array->n_stored) {
      mrb_value mrbc_array = GET_ARY_ARG(1);
      for (size_t i = 0; i < 3; i++) {
        mrb_value value = mrbc_array_get(&mrbc_array, i);
        c_array[i] = value.i;
      }
    }
  }
}

/**
 * @brief Sample method for creating and returning arrays
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_sample_array3(mrb_vm *vm, mrb_value *v, int argc) {
  float tmp[3] = {0.0f, 1.0f, 3.14f};
  mrb_value ret = mrbc_array_new(vm, 3);

  for (int i = 0; i < 3; i++) {
    mrb_value v = mrbc_float_value(vm, tmp[i]);
    mrbc_array_set(&ret, i, &v);
  }

  SET_RETURN(ret);
}

/**
 * @brief Gets the reload status from the mruby/c VM
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc) {
  const uint8_t kVmId = (vm->vm_id - 1);
  if (MAX_VM_COUNT > kVmId) {
    watchdog_feed[kVmId] = true;
    watchdog_enable[kVmId] = true;
  }
  SET_BOOL_RETURN(app_mrubyc_vm_get_reload());
}

/**
 * @brief Initializes the Blink subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_blink_init(void) {
  for (size_t i = 0; i < MAX_VM_COUNT; i++) {
    watchdog_enable[i] = false;
    watchdog_feed[i] = true;
  }
  return kSuccess;
}

/**
 * @brief Performs normality check for watchdog functionality
 *
 * @return fn_t kSuccess if all VMs are responding, kFailure otherwise
 */
fn_t api_blink_normality_check(void) {
  fn_t ret = kSuccess;
  for (size_t i = 0; i < MAX_VM_COUNT; i++) {
    if (true == watchdog_enable[i]) {
      if (true == watchdog_feed[i]) {
        watchdog_feed[i] = false;
      } else {
        ret = kFailure;
      }
    }
  }
  return ret;
}
