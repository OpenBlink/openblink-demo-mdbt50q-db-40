/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file gpio.c
 * @brief Implementation of GPIO driver
 * @details Implements functions for controlling LEDs and reading switch states
 */
#include "gpio.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "../lib/fn.h"

LOG_MODULE_REGISTER(drv_gpio, LOG_LEVEL_DBG);

/** @brief GPIO specifications for switches */
static const struct gpio_dt_spec kSw[4] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(sw2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(sw3), gpios)};

/** @brief GPIO specifications for LEDs */
static const struct gpio_dt_spec kLed[3] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios)};

/**
 * @brief Initializes the GPIO subsystem
 *
 * @details Configures switch pins as inputs and LED pins as outputs
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_gpio_init(void) {
  fn_t tmp_ret = kSuccess;
  // input
  for (size_t i = 0; i < sizeof(kSw) / sizeof(kSw[0]); i++) {
    if (true == gpio_is_ready_dt(&kSw[i])) {
      if (0 > gpio_pin_configure_dt(&kSw[i], GPIO_INPUT)) {
        tmp_ret = kFailure;
        LOG_ERR("Failed to configure GPIO %d", i);
      }
    } else {
      tmp_ret = kFailure;
      LOG_ERR("Failed to get GPIO %d", i);
    }
  }
  // output
  for (size_t i = 0; i < sizeof(kLed) / sizeof(kLed[0]); i++) {
    if (true == gpio_is_ready_dt(&kLed[i])) {
      if (0 > gpio_pin_configure_dt(&kLed[i], GPIO_OUTPUT_INACTIVE)) {
        tmp_ret = kFailure;
        LOG_ERR("Failed to configure LED %d", i);
      }
    } else {
      tmp_ret = kFailure;
      LOG_ERR("Failed to get LED %d", i);
    }
  }

  return tmp_ret;
}

/**
 * @brief Gets the current state of a GPIO pin
 *
 * @param kTgt Target GPIO pin
 * @return true if the pin is active (switch pressed)
 * @return false if the pin is inactive (switch released)
 */
bool drv_gpio_get(const drv_gpio_t kTgt) {
  switch (kTgt) {
    case kDrvGpioSW1:
      if (1 == gpio_pin_get_dt(&kSw[0])) {
        return true;
      }
      break;
    case kDrvGpioSW2:
      if (1 == gpio_pin_get_dt(&kSw[1])) {
        return true;
      }
      break;
    case kDrvGpioSW3:
      if (1 == gpio_pin_get_dt(&kSw[2])) {
        return true;
      }
      break;
    case kDrvGpioSW4:
      if (1 == gpio_pin_get_dt(&kSw[3])) {
        return true;
      }
      break;
    default:
      return false;
  }
  return false;
}

/**
 * @brief Sets the state of a GPIO pin
 *
 * @param kTgt Target GPIO pin
 * @param kReq Requested state (true for active, false for inactive)
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_gpio_set(const drv_gpio_t kTgt, const bool kReq) {
  int tmp_value = kReq ? 1 : 0;
  switch (kTgt) {
    case kDrvGpioLED1:
      gpio_pin_set_dt(&kLed[0], tmp_value);
      return kSuccess;
    case kDrvGpioLED2:
      gpio_pin_set_dt(&kLed[1], tmp_value);
      return kSuccess;
    case kDrvGpioLED3:
      gpio_pin_set_dt(&kLed[2], tmp_value);
      return kSuccess;
    default:
      return kFailure;
  }
  return kFailure;
}
