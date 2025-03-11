/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file adc.h
 * @brief ADC driver interface
 * @details Provides functions for ADC initialization and reading
 */
#ifndef DRV_ADC_H
#define DRV_ADC_H

#include <stdint.h>

#include "../lib/fn.h"

/**
 * @brief Initializes the ADC subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_adc_init(void);

/**
 * @brief Updates all ADC channel readings
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t drv_adc_update(void);

/**
 * @brief Gets the value of an ADC channel
 *
 * @param kIdx Index of the ADC channel to read
 * @return int32_t ADC reading in millivolts, or negative on error
 */
int32_t drv_adc_get(const uint8_t kIdx);

#endif
