/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file led.h
 * @brief LED API for mruby/c
 * @details Defines the LED class and methods for mruby/c scripts to control
 * LEDs
 */
#ifndef API_LED_H
#define API_LED_H

#include "../lib/fn.h"

/**
 * @brief Defines the LED class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_led_define(void);

#endif
