/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file comm.h
 * @brief Communication management interface
 * @details Defines functions for managing BLE communication
 */
#ifndef APP_COMM_H
#define APP_COMM_H

#include <stdbool.h>

#include "../lib/fn.h"

/**
 * @brief Initializes the communication subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t comm_init(void);

/**
 * @brief Checks if BLE advertising is active
 *
 * @return true if advertising is active
 * @return false if advertising is not active
 */
bool comm_get_advertising(void);

/**
 * @brief Checks if BLE is connected to a device
 *
 * @return true if connected
 * @return false if not connected
 */
bool comm_get_connected(void);

/**
 * @brief Disconnects the current BLE connection
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t comm_disconnect(void);

#endif  // APP_COMM_H
