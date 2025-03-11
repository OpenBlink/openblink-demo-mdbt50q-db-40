/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file main.c
 * @brief Main entry point for the OpenBlink firmware
 * @details This file contains the main function and factory reset detection
 * logic
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

/**
 * @brief Main function of the OpenBlink firmware
 *
 * @return EXIT_FAILURE if the program exits unexpectedly
 */
int main(void) {
  while (1) {
    judge_factory_reset();
    k_msleep(100);
  }
  return EXIT_FAILURE;
}

/**
 * @brief Detects if factory reset is requested by monitoring button presses
 *
 * @details Monitors SW1 and SW4 buttons. If both are pressed for a certain
 * duration, it triggers a factory reset and reboots the device.
 *          - 5-10 seconds: LED2 blinks to indicate pending factory reset
 *          - >10 seconds: Factory reset is performed, LED2 stays on for 1
 * second
 */
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
