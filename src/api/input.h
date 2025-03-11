/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file input.h
 * @brief Input API for mruby/c
 * @details Defines the Input class and methods for mruby/c scripts to handle
 * button inputs
 */
#ifndef API_INPUT_H
#define API_INPUT_H

#include "../lib/fn.h"

/**
 * @brief Defines the Input class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_input_define(void);

#endif
