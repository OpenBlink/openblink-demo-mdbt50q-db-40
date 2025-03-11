/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file api.h
 * @brief API utility functions for mruby/c
 * @details Provides utility functions for handling mruby/c data types
 */
#ifndef API_API_H
#define API_API_H

#include <stdbool.h>
#include <stdint.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

/**
 * @brief Converts mruby/c boolean type to C boolean
 *
 * @param kType The mruby/c value type to convert
 * @return true if the type is MRBC_TT_TRUE
 * @return false if the type is MRBC_TT_FALSE or invalid
 */
bool api_api_get_bool(const mrbc_vtype kType);

/**
 * @brief Sets a system task for a specific VM
 *
 * @param kVmId The VM ID to set as system task
 * @param kBytecode The bytecode for the system task
 * @param kHmac The HMAC for verification
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_api_set_systemtask(const uint8_t kVmId, const uint8_t *const kBytecode,
                            const uint8_t *const kHmac);

/**
 * @brief Checks if a VM ID is the system task
 *
 * @param kVmId The VM ID to check
 * @return fn_t kSuccess if it is the system task, kFailure otherwise
 */
fn_t api_api_check_systemtask(const uint8_t kVmId);

/**
 * @brief Checks if expert API access is allowed
 *
 * @return fn_t kSuccess if expert API is allowed, kFailure otherwise
 */
fn_t api_api_get_allow_expert_api(void);

#endif
