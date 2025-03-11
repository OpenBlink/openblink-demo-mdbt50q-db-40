/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file die_temperature.c
 * @brief Implementation of die temperature sensor interface
 * @details Implements functions for reading the die temperature from the Nordic
 * nRF chip
 */
#include "die_temperature.h"

#include <stdbool.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../../lib/fn.h"

LOG_MODULE_REGISTER(hal_die_temperature, LOG_LEVEL_WRN);

static const struct device* kDevDieTemp = DEVICE_DT_GET_ANY(nordic_nrf_temp);

/**
 * @brief Gets the current die temperature
 *
 * @details Reads the temperature from the Nordic nRF temperature sensor
 *
 * @return float Temperature in degrees Celsius, or INT32_MIN on error
 */
float hal_die_temperature_get(void) {
  struct sensor_value temp_val;
  if (true == device_is_ready(kDevDieTemp)) {
    int ret;
    ret = sensor_sample_fetch(kDevDieTemp);
    if (0 != ret) {
      LOG_ERR("sensor_sample_fetch() failed: %d", ret);
      return INT32_MIN;
    }
    ret = sensor_channel_get(kDevDieTemp, SENSOR_CHAN_DIE_TEMP, &temp_val);
    if (0 != ret) {
      LOG_ERR("sensor_channel_get() failed: %d", ret);
      return INT32_MIN;
    }
    LOG_DBG("%d, %d", temp_val.val1, temp_val.val2);
  }

  return ((float)temp_val.val1 + ((float)temp_val.val2 / 1000000.0f));
}
