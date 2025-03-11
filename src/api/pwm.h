/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file pwm.h
 * @brief PWM API for mruby/c
 * @details Defines the PWM class and methods for mruby/c scripts
 */
#ifndef API_PWM_H
#define API_PWM_H

#include "../lib/fn.h"

/**
 * @brief Defines the PWM class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_pwm_define(void);

#endif
