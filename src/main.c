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
#include "app/watchdog.h"
#include "drv/gpio.h"
#include "lib/hmac-sha256.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static void judge_factory_reset(void);
static void debug_lib_hmac(void);

/**
 * @brief Main function of the OpenBlink firmware
 *
 * @return EXIT_FAILURE if the program exits unexpectedly
 */
int main(void) {
  debug_lib_hmac();
  while (1) {
    watchdog_thread_hearbeat(kAppWatchDogThreadMain);
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
 *          - 3-5 seconds: LED2 blinks to indicate pending factory reset
 *          - >5 seconds: Factory reset is performed, LED2 stays on for 1
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
  if ((30 <= duration) && (50 > duration)) {
    if (0 == (duration % 2)) {
      drv_gpio_set(kDrvGpioLED2, true);
    } else {
      drv_gpio_set(kDrvGpioLED2, false);
    }
  } else if (50 <= duration) {
    if (false == factory_reset_flag) {
      factory_reset_flag = true;
      init_factory_reset();
      drv_gpio_set(kDrvGpioLED2, true);
      k_msleep(1000);
    }
  }
}

// **************************************************************************
// debug_lib_hmac
static void debug_lib_hmac(void) {
  uint8_t dummy_data[32] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                            11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                            22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

  hmac_sha256_key_t key = {
      .value = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      .id = 0,
      .len = 0};

  hmac_sha256_hmac_t tmp_hmac = {
      .value = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      .len = 0};

  // Key generation
  hmac_sha256_genkey(&key);

  // HAMC-SHA256 sign
  hmac_sha256_sign(&key, &tmp_hmac, dummy_data,
                   sizeof(dummy_data) / sizeof(dummy_data[0]));

  // HMAC-SHA256 verification
  if (kSuccess !=
      hmac_sha256_verify(&key, &tmp_hmac, dummy_data,
                         sizeof(dummy_data) / sizeof(dummy_data[0]))) {
    LOG_DBG("HMAC-SHA256 verification failed!");
  } else {
    LOG_DBG("HMAC-SHA256 verification successful!");
  }
}
