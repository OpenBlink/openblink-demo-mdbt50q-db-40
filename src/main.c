/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app/init.h"
#include "drv/gpio.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static void judge_factory_reset(void);

// **************************************************************************
// main
int main(void) {
  while (1) {
    judge_factory_reset();
    k_msleep(100);
  }
  return EXIT_FAILURE;
}

// **************************************************************************
// judge_factory_reset
static void judge_factory_reset(void) {
  static bool factory_reset_flag = false;
  static uint16_t duration = 0;
  if ((true == drv_gpio_get(kDrvGpioSW1)) &&
      (true == drv_gpio_get(kDrvGpioSW4))) {
    duration += 1;
  } else {
    duration = 0;
    drv_gpio_set(kDrvGpioLED2, false);
    if (true == factory_reset_flag) {
      drv_gpio_set(kDrvGpioLED2, false);
      init_reboot();
    }
  }
  if ((50 <= duration) && (100 > duration)) {
    if (0 == (duration % 2)) {
      drv_gpio_set(kDrvGpioLED2, true);
    } else {
      drv_gpio_set(kDrvGpioLED2, false);
    }
  } else if (100 <= duration) {
    if (false == factory_reset_flag) {
      factory_reset_flag = true;
      init_factory_reset();
      drv_gpio_set(kDrvGpioLED2, true);
      k_msleep(1000);
    }
  }
}
