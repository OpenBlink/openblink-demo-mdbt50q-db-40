/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file ble.h
 * @brief BLE API for mruby/c
 * @details Defines the BLE class and methods for mruby/c scripts
 */
#ifndef API_BLE_H
#define API_BLE_H

#include "../lib/fn.h"

/**
 * @brief Defines the BLE class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_ble_define(void);

#endif
