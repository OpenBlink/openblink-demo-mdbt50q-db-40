/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef DRV_ADC_H
#define DRV_ADC_H

#include <stdint.h>

#include "../lib/fn.h"

fn_t drv_adc_init(void);
fn_t drv_adc_update(void);
int32_t drv_adc_get(const uint8_t kIdx);

#endif
