/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "blink.h"

#include "../../mrubyc/src/mrubyc.h"
#include "../app/mrubyc_vm.h"
#include "../lib/fn.h"
static bool watchdog_enable[MAX_VM_COUNT];
static bool watchdog_feed[MAX_VM_COUNT];

// **************************************************************************
// forward declaration
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc);
static void c_sample_string(mrb_vm *vm, mrb_value *v, int argc);
static void c_sample_array(mrb_vm *vm, mrb_value *v, int argc);
static void c_sample_array2(mrb_vm *vm, mrb_value *v, int argc);
static void c_sample_array3(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_blink_define
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

// **************************************************************************
// c_sample_string
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

// **************************************************************************
// c_sample_array
static void c_sample_array(mrb_vm *vm, mrb_value *v, int argc) {
  uint8_t array[3] = {0};
  if (MRBC_TT_ARRAY == v[1].tt) {
    if (3 != v[1].array->n_stored) return;
    for (size_t i = 0; i < 3; i++) {
      array[i] = (uint8_t)v[1].array->data[i].i;
    }
  }
}

// **************************************************************************
// c_sample_array2
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

// **************************************************************************
// c_sample_array3
static void c_sample_array3(mrb_vm *vm, mrb_value *v, int argc) {
  float tmp[3] = {0.0f, 1.0f, 3.14f};
  mrb_value ret = mrbc_array_new(vm, 3);

  for (int i = 0; i < 3; i++) {
    mrb_value v = mrbc_float_value(vm, tmp[i]);
    mrbc_array_set(&ret, i, &v);
  }

  SET_RETURN(ret);
}

// **************************************************************************
// c_get_reload
static void c_get_reload(mrb_vm *vm, mrb_value *v, int argc) {
  const uint8_t kVmId = (vm->vm_id - 1);
  if (MAX_VM_COUNT > kVmId) {
    watchdog_feed[kVmId] = true;
    watchdog_enable[kVmId] = true;
  }
  SET_BOOL_RETURN(app_mrubyc_vm_get_reload());
}

// **************************************************************************
// api_blink_init
fn_t api_blink_init(void) {
  for (size_t i = 0; i < MAX_VM_COUNT; i++) {
    watchdog_enable[i] = false;
    watchdog_feed[i] = true;
  }
  return kSuccess;
}

// **************************************************************************
// api_blink_normality_check
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
