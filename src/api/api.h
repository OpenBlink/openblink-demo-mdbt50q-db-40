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

#include "../../mrubyc/src/mrubyc.h"

/**
 * @brief Converts mruby/c boolean type to C boolean
 *
 * @param kType The mruby/c value type to convert
 * @return true if the type is MRBC_TT_TRUE
 * @return false if the type is MRBC_TT_FALSE or invalid
 */
bool api_api_get_bool(const mrbc_vtype kType);

#endif
