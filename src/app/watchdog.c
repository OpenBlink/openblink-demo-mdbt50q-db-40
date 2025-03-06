/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "watchdog.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../api/blink.h"
#include "../lib/fn.h"

LOG_MODULE_REGISTER(app_watchdog, LOG_LEVEL_WRN);

#define WDT_OPT WDT_OPT_PAUSE_HALTED_BY_DBG
#define WDT_MAX_WINDOW_MS 10000U
#define WDT_MIN_WINDOW_MS 0U

static const struct device *wdt_dev = DEVICE_DT_GET(DT_NODELABEL(wdt));
static int wdt_channel_id = 0;

static void watchdog_thread_main(void *, void *, void *);
K_THREAD_DEFINE(thread_watchdog, 384, watchdog_thread_main, NULL, NULL, NULL,
                10, K_ESSENTIAL, 100);

static void watchdog_work_queue_watch(struct k_work *const work);
K_WORK_DEFINE(work_watchdog, watchdog_work_queue_watch);

static bool thread_heartbeat[kAppWatchDogThreadNum] = {true};

// **************************************************************************
// watchdog_init
fn_t watchdog_init(void) {
  int err = 0;
  struct wdt_timeout_cfg wdt_config = {
      .flags = WDT_FLAG_RESET_SOC,
      .window.min = WDT_MIN_WINDOW_MS,
      .window.max = WDT_MAX_WINDOW_MS,
  };

  if (!device_is_ready(wdt_dev)) {
    LOG_ERR("Watchdog: device not ready.");
    return kFailure;
  }

  wdt_channel_id = wdt_install_timeout(wdt_dev, &wdt_config);
  if (0 > wdt_channel_id) {
    LOG_ERR("Watchdog install error: %d", wdt_channel_id);
    return kFailure;
  }

  err = wdt_setup(wdt_dev, WDT_OPT);
  if (0 != err) {
    LOG_ERR("Watchdog setup error: %d", err);
    return kFailure;
  }

  return kSuccess;
}

// **************************************************************************
// watchdog_get_max_window_ms
uint32_t watchdog_get_max_window_ms(void) { return WDT_MAX_WINDOW_MS; }

// **************************************************************************
// watchdog_thread_main
static void watchdog_thread_main(void *, void *, void *) {
  while (1) {
    fn_t tmp_state = kSuccess;

    // mruby/c
    tmp_state =
        (kSuccess != api_blink_normality_check()) ? kFailure : tmp_state;

    // Thread heartbeat check
    for (size_t i = 0; kAppWatchDogThreadNum > i; i++) {
      if (true == thread_heartbeat[i]) {
        thread_heartbeat[i] = false;
      } else {
        tmp_state = kFailure;
      }
    }

    // Watchdog feed
    if (kSuccess == tmp_state) {
      LOG_DBG("Feeding watchdog...");
      const int kErr = wdt_feed(wdt_dev, wdt_channel_id);
      if (0 != kErr) {
        LOG_ERR("Watchdog feed error: %d", kErr);
      }
    }

    // System work queue watchdog
    k_work_submit(&work_watchdog);

    k_msleep(watchdog_get_max_window_ms() / 4);
  }
}

// **************************************************************************
// watchdog_thread_hearbeat
fn_t watchdog_thread_hearbeat(const app_watchdog_thread_t kThreadName) {
  if (kAppWatchDogThreadNum <= kThreadName) {
    return kFailure;
  }
  thread_heartbeat[kThreadName] = true;
  return kSuccess;
}

// **************************************************************************
// watchdog_work_queue_watch
static void watchdog_work_queue_watch(struct k_work *const work) {
  watchdog_thread_hearbeat(kAppWatchDogThreadSysWorkQ);
}
