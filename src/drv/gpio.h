/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef DRV_GPIO_H
#define DRV_GPIO_H

#include <stdbool.h>

#include "../lib/fn.h"

typedef enum {
  kDrvGpioSW1,
  kDrvGpioSW2,
  kDrvGpioSW3,
  kDrvGpioSW4,
  kDrvGpioLED1,
  kDrvGpioLED2,
  kDrvGpioLED3,
} drv_gpio_t;

fn_t drv_gpio_init(void);
bool drv_gpio_get(const drv_gpio_t kTgt);
fn_t drv_gpio_set(const drv_gpio_t kTgt, const bool kReq);

#endif
