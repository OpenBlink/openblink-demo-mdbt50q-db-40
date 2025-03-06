/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef DRV_PWM_H
#define DRV_PWM_H

#include <stdint.h>

#include "../lib/fn.h"

fn_t drv_pwm_set(const uint32_t kHz, const uint8_t kDuty);

#endif
