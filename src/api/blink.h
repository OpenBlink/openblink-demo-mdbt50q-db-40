/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.h
 * @brief Blink API for mruby/c
 * @details Defines the Blink class and methods for mruby/c scripts
 */
#ifndef API_BLINK_H
#define API_BLINK_H

#include "../lib/fn.h"

fn_t api_blink_init(void);

/**
 * @brief Defines the Blink class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_blink_define(void);
fn_t api_blink_normality_check(void);

#endif
