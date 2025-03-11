/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file i2c.h
 * @brief I2C API for mruby/c
 * @details Defines the I2C class and methods for mruby/c scripts
 */
#ifndef API_I2C_H
#define API_I2C_H

#include "../lib/fn.h"

/**
 * @brief Initializes the I2C subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_i2c_init(void);

/**
 * @brief Defines the I2C class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_i2c_define(void);

#endif
