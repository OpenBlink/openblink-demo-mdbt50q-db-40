/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file ble.c
 * @brief Implementation of BLE API for mruby/c
 * @details Implements the BLE class and methods for mruby/c scripts
 */
#include "ble.h"

#include "../../mrubyc/src/mrubyc.h"
#include "../app/comm.h"
#include "../lib/fn.h"

/**
 * @brief Forward declaration for BLE state getter method
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_ble(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the BLE class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_ble_define(void) {
  mrb_class *class_ble;
  class_ble = mrbc_define_class(0, "BLE", mrbc_class_object);
  mrbc_define_method(0, class_ble, "state", c_get_ble);
  return kSuccess;
}

/**
 * @brief Gets the current BLE state
 *
 * @details Returns an integer representing the current BLE state:
 *          - 0: Off
 *          - 1: Advertising
 *          - 2: Connected
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_get_ble(mrb_vm *vm, mrb_value *v, int argc) {
  enum {
    kOff = 0,         /**< BLE is off */
    kAdvertising = 1, /**< BLE is advertising */
    kConnected = 2,   /**< BLE is connected */
  };
  if (true == comm_get_advertising()) {
    SET_INT_RETURN(kAdvertising);
  } else if (true == comm_get_connected()) {
    SET_INT_RETURN(kConnected);
  } else {
    SET_INT_RETURN(kOff);
  }
}
