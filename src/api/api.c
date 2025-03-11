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
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"

LOG_MODULE_REGISTER(api_api, LOG_LEVEL_DBG);

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
