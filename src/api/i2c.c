/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file i2c.c
 * @brief Implementation of I2C API for mruby/c
 * @details Implements the I2C class and methods for mruby/c scripts
 */
#include "i2c.h"

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

LOG_MODULE_REGISTER(api_i2c, LOG_LEVEL_WRN);

static const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));

/**
 * @brief Forward declarations for I2C methods
 */
static void c_i2c_read(mrb_vm *vm, mrb_value *v, int argc);
static void c_i2c_write(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Initializes the I2C subsystem
 *
 * @details Configures the I2C device with fast speed setting
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_i2c_init(void) {
  if (!device_is_ready(i2c_dev)) {
    LOG_ERR("I2C device is not ready");
    return kFailure;
  }
  const int kRc = i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST));
  if (0 != kRc) {
    LOG_ERR("Failed to configure I2C device");
    return kFailure;
  }

  LOG_INF("I2C device is ready");
  return kSuccess;
}

/**
 * @brief Defines the I2C class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_i2c_define(void) {
  mrb_class *class_i2c;
  class_i2c = mrbc_define_class(0, "I2C", mrbc_class_object);
  mrbc_define_method(0, class_i2c, "read", c_i2c_read);
  mrbc_define_method(0, class_i2c, "write", c_i2c_write);
  return kSuccess;
}

/**
 * @brief Reads data from an I2C device
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_i2c_read(mrb_vm *vm, mrb_value *v, int argc) {
  uint16_t device_id = 0U;
  uint8_t address = 0U;
  uint8_t size = 0U;
  uint8_t data_buf[255] = {0U};

  // DeviceID
  if (true == MRBC_ISNUMERIC(v[1])) {
    device_id = GET_INT_ARG(1);
  }
  // Address
  if (true == MRBC_ISNUMERIC(v[2])) {
    address = GET_INT_ARG(2);
  }
  // Size
  if (true == MRBC_ISNUMERIC(v[3])) {
    size = GET_INT_ARG(3);
    if ((sizeof(data_buf) - 1) < size) return;
  }

  mrb_value ret = mrbc_array_new(vm, size + 1);
  const int kRc =
      i2c_write_read(i2c_dev, device_id, &address, 1, data_buf, size);
  LOG_DBG("i2c_read: return:%d, ID:0x%02X, Address:0x%02X, Size:0x%02X", kRc,
          device_id, address, size);

  // ReturnCode
  do {
    mrb_value v = mrbc_integer_value(kRc);
    mrbc_array_set(&ret, 0, &v);
  } while (0);

  // Data
  for (int i = 0; i < size; i++) {
    if (0x00U == kRc) {
      mrb_value v = mrbc_integer_value(data_buf[i]);
      mrbc_array_set(&ret, i + 1, &v);
    } else {
      mrb_value v = mrbc_integer_value(0);
      mrbc_array_set(&ret, i + 1, &v);
    }
  }

  SET_RETURN(ret);
}

/**
 * @brief Writes data to an I2C device
 *
 * @param vm The mruby/c VM instance
 * @param v The value array
 * @param argc The argument count
 */
static void c_i2c_write(mrb_vm *vm, mrb_value *v, int argc) {
  uint16_t device_id = 0U;
  uint8_t size = 0U;
  uint8_t data_buf[255] = {0U};

  // DeviceID
  if (true == MRBC_ISNUMERIC(v[1])) {
    device_id = GET_INT_ARG(1);
  }
  // Address
  if (true == MRBC_ISNUMERIC(v[2])) {
    data_buf[0] = GET_INT_ARG(2);
  }
  // Data
  if (MRBC_TT_ARRAY == v[3].tt) {
    size = v[3].array->n_stored;
    if (((sizeof(data_buf) / sizeof(data_buf[0])) - 1) < size) return;
    for (size_t i = 0; i < size; i++) {
      data_buf[i + 1] = (uint8_t)v[3].array->data[i].i;
    }
  }

  const int kRc =
      i2c_write(i2c_dev, data_buf, (uint32_t)(size + 1), (uint16_t)device_id);
  LOG_DBG("i2c_write: return:%d, ID:0x%02X, Address:0x%02X, Size:0x%02X", kRc,
          device_id, data_buf[0], size);

  SET_INT_RETURN(kRc);
}
