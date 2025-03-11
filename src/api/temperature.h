/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file temperature.h
 * @brief Temperature API for mruby/c
 * @details Defines the Temperature class and methods for mruby/c scripts
 */
#ifndef API_TEMPERATURE_H
#define API_TEMPERATURE_H

#include "../lib/fn.h"

/**
 * @brief Defines the Temperature class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_temperature_define(void);

#endif
