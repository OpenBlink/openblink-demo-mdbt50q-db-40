/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "api.h"

#include <stdbool.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"

LOG_MODULE_REGISTER(api_api, LOG_LEVEL_DBG);

// **************************************************************************
// api_api_get_bool
bool api_api_get_bool(const mrbc_vtype kType) {
  if (MRBC_TT_TRUE == kType) {
    return true;
  } else if (MRBC_TT_FALSE == kType) {
    return false;
  }
  LOG_ERR("api_get_bool: Invalid type.");
  return false;
}
