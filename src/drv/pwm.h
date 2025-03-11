/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file pwm.h
 * @brief PWM driver interface
 * @details Provides functions for PWM control
 */
#ifndef DRV_PWM_H
#define DRV_PWM_H

#include <stdint.h>

#include "../lib/fn.h"

/**
 * @brief Sets the PWM frequency and duty cycle
 *
 * @param kHz Frequency in kHz
 * @param kDuty Duty cycle (0-100)
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_pwm_set(const uint32_t kHz, const uint8_t kDuty);

#endif
