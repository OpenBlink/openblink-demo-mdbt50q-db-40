/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file pwm.c
 * @brief Implementation of PWM driver
 * @details Implements functions for PWM control
 */
#include "pwm.h"

#include <stdbool.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../lib/fn.h"

LOG_MODULE_REGISTER(drv_pwm, LOG_LEVEL_DBG);

static const struct device *pwm1_dev = DEVICE_DT_GET(DT_NODELABEL(pwm1));

/**
 * @brief Sets the PWM frequency and duty cycle
 *
 * @param kHz Frequency in kHz
 * @param kDuty Duty cycle (0-100)
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_pwm_set(const uint32_t kHz, const uint8_t kDuty) {
  int err;
  if (100 < kDuty) {
    LOG_ERR("duty must be less than 100\n");
    return kFailure;
  }
  uint32_t kPeriod = (1000U * 1000U * 1000U) / (uint32_t)kHz;
  uint32_t kPulse = (kPeriod * kDuty) / 100U;
  if (true == device_is_ready(pwm1_dev)) {
    err = pwm_set(pwm1_dev, 0U, kPeriod, kPulse, PWM_POLARITY_NORMAL);
    if (0 == err) {
      return kSuccess;
    } else {
      LOG_ERR("pwm_set() failed with err %d\n", err);
    }
  }
  return kFailure;
}
