/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file config.h
 * @brief Configuration management interface
 * @details Provides functions for managing device configuration settings
 */
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "../lib/fn.h"

/**
 * @brief Initializes the configuration subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t config_init(void);

#endif  // APP_CONFIG_H
