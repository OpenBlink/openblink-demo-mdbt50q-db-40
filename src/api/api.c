/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file api.c
 * @brief Implementation of API utility functions for mruby/c
 * @details Provides utility functions for handling mruby/c data types
 */
#include "api.h"

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"

LOG_MODULE_REGISTER(api_api, LOG_LEVEL_DBG);

static uint8_t system_task_vmid = 0U;

/**
 * @brief Converts mruby/c boolean type to C boolean
 *
 * @param kType The mruby/c value type to convert
 * @return true if the type is MRBC_TT_TRUE
 * @return false if the type is MRBC_TT_FALSE or invalid
 */
bool api_api_get_bool(const mrbc_vtype kType) {
  if (MRBC_TT_TRUE == kType) {
    return true;
  } else if (MRBC_TT_FALSE == kType) {
    return false;
  }
  LOG_ERR("api_get_bool: Invalid type.");
  return false;
}

/**
 * @brief Sets a system task for a specific VM
 *
 * @param kVmId The VM ID to set as system task
 * @param kBytecode The bytecode for the system task
 * @param kHmac The HMAC for verification
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_api_set_systemtask(const uint8_t kVmId, const uint8_t *const kBytecode,
                            const uint8_t *const kHmac) {
  // ToDo: HMAC verify
  system_task_vmid = kVmId;
  LOG_DBG("SystemTask's vm_id is set to %d.", kVmId);
  return kSuccess;
}

/**
 * @brief Checks if a VM ID is the system task
 *
 * @param kVmId The VM ID to check
 * @return fn_t kSuccess if it is the system task, kFailure otherwise
 */
fn_t api_api_check_systemtask(const uint8_t kVmId) {
  if (kVmId == system_task_vmid) {
    return kSuccess;
  } else {
    return kFailure;
  }
}

/**
 * @brief Checks if expert API access is allowed
 *
 * @return fn_t kSuccess if expert API is allowed, kFailure otherwise
 */
fn_t api_api_get_allow_expert_api(void) {
#if CONFIG_DEBUG
  LOG_DBG("API: Debug mode is enabled.");
  return kSuccess;
#endif
  // ToDo: Implement this function
  return kFailure;
}
