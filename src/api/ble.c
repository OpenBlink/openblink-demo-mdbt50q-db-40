/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "ble.h"

#include "../../mrubyc/src/mrubyc.h"
#include "../app/comm.h"
#include "../lib/fn.h"

// **************************************************************************
// forward declaration
static void c_get_ble(mrb_vm *vm, mrb_value *v, int argc);

// **************************************************************************
// api_ble_define
fn_t api_ble_define(void) {
  mrb_class *class_ble;
  class_ble = mrbc_define_class(0, "BLE", mrbc_class_object);
  mrbc_define_method(0, class_ble, "state", c_get_ble);
  return kSuccess;
}

// **************************************************************************
// c_get_ble
static void c_get_ble(mrb_vm *vm, mrb_value *v, int argc) {
  enum {
    kOff = 0,
    kAdvertising = 1,
    kConnected = 2,
  };
  if (true == comm_get_advertising()) {
    SET_INT_RETURN(kAdvertising);
  } else if (true == comm_get_connected()) {
    SET_INT_RETURN(kConnected);
  } else {
    SET_INT_RETURN(kOff);
  }
}
