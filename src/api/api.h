/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef API_API_H
#define API_API_H

#include <stdbool.h>
#include <stdint.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

bool api_api_get_bool(const mrbc_vtype kType);
fn_t api_api_set_systemtask(const uint8_t kVmId, const uint8_t *const kBytecode,
                            const uint8_t *const kHmac);
fn_t api_api_check_systemtask(const uint8_t kVmId);
fn_t api_api_get_allow_expert_api(void);

#endif
