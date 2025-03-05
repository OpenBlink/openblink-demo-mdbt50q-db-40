/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/reboot.h>

#include "../api/symbol.h"
#include "../drv/gpio.h"
#include "../lib/fn.h"
#include "app_version.h"
#include "blink.h"
#include "comm.h"
#include "mrubyc_vm.h"
#include "ncs_version.h"
#include "storage.h"
#include "version.h"

LOG_MODULE_REGISTER(app_init, LOG_LEVEL_DBG);

extern struct k_mutex mutex_storage;

static int init_main(void);
SYS_INIT(init_main, APPLICATION, 0);

// **************************************************************************
// init_main
static int init_main(void) {
  int64_t timestamp = k_uptime_get();
  fn_t ret = kSuccess;
  uint32_t reset_cause = 0x00U;
  uint8_t buf[8] = {0x00U};
  char device_name[BLINK_DEVICE_NAME_SIZE] = {0};

  // ==============================
  // Boot banner
  hwinfo_get_reset_cause(&reset_cause);
  hwinfo_get_device_id(buf, (sizeof(buf) / sizeof(buf[0])));
  blink_get_name(device_name, sizeof(device_name));
  LOG_INF("DeviceName: %s", device_name);
  LOG_INF("DeviceID: 0x%02X%02X%02X%02X%02X%02X%02X%02X", buf[7], buf[6],
          buf[5], buf[4], buf[3], buf[2], buf[1], buf[0]);
  LOG_INF("App Ver: %s (%s)", APP_VERSION_EXTENDED_STRING,
          STRINGIFY(APP_BUILD_VERSION));
  LOG_INF("nRF Connect SDK Ver: %s", NCS_VERSION_STRING);
  LOG_INF("Zephyr Ver: %s", STRINGIFY(BUILD_VERSION));
  LOG_INF("Reset cause: 0x%08X", reset_cause);

  // ==============================
  // Initialize
  LOG_INF("zms_storage init");
  ret = (kSuccess != storage_init()) ? kFailure : ret;
  LOG_INF("settings_storage init");
  ret = (0 != settings_subsys_init()) ? kFailure : ret;
  storage_free_space();
  LOG_INF("ZMS storage max data size: %zu", storage_maximum_data_size());
  ret = (kSuccess != api_symbol_init()) ? kFailure : ret;
  ret = (kSuccess != drv_gpio_init()) ? kFailure : ret;
  ret = (kSuccess != comm_init()) ? kFailure : ret;

  // ==============================
  // Result
  if (kSuccess == ret) {
    LOG_INF("=== Init. Succeeded! (%lli ms) ===", k_uptime_delta(&timestamp));
    return EXIT_SUCCESS;
  } else {
    LOG_ERR("=== Init. FAILED (%lli ms) ===", k_uptime_delta(&timestamp));
    return EXIT_FAILURE;
  }
}

// **************************************************************************
// init_reboot
fn_t init_reboot(void) {
  LOG_WRN("Rebooting...");
  // Reboot
  for (uint8_t i = 0; 10 > i; i++) {
    if (0 == k_mutex_lock(&mutex_storage, K_MSEC(100))) {
      sys_reboot(SYS_REBOOT_WARM);
      k_msleep(100);
      k_mutex_unlock(&mutex_storage);
    } else {
      LOG_ERR("Failed to lock mutex_storage");
    }
    k_yield();
  }
  // Force reboot
  sys_reboot(SYS_REBOOT_COLD);
  return kUndetermined;
}

// **************************************************************************
// init_factory_reset
fn_t init_factory_reset(void) {
  blink_delete(kBlinkSlot1);
  blink_delete(kBlinkSlot2);
  return kSuccess;
}
