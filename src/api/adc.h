/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file adc.h
 * @brief ADC API for mruby/c
 * @details Defines the ADC class and methods for mruby/c scripts
 */
#ifndef API_ADC_H
#define API_ADC_H

#include "../lib/fn.h"

/**
 * @brief Defines the ADC class and methods for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_adc_define(void);

#endif
