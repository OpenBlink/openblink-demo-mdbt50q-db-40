/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file init.h
 * @brief System initialization and reset functions
 * @details Provides functions for factory reset and system reboot
 */
#ifndef APP_INIT_H
#define APP_INIT_H

#include "../lib/fn.h"

/**
 * @brief Reboots the device
 *
 * @details Performs a software reset of the system
 *
 * @return fn_t kSuccess if successful, kUndetermined if reboot fails
 */
fn_t init_reboot(void);

/**
 * @brief Performs a factory reset of the device
 *
 * @details Clears all stored bytecode and returns the device to its initial
 * state
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t init_factory_reset(void);

#endif
