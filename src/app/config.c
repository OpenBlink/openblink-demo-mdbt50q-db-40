/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "config.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

#include "../lib/fn.h"

LOG_MODULE_REGISTER(app_config, LOG_LEVEL_WRN);

int config_handle_get(const char *name, char *val, int val_len_max);
int config_handle_set(const char *name, size_t len, settings_read_cb read_cb,
                      void *cb_arg);
int config_handle_export(int (*cb)(const char *name, const void *value,
                                   size_t val_len));

struct settings_handler config_handler = {.name = "openblink",
                                          .h_get = config_handle_get,
                                          .h_set = config_handle_set,
                                          .h_commit = NULL,
                                          .h_export = config_handle_export};

static uint32_t blink_count_trip;        // volatile
static uint32_t blink_count_total = 0U;  // non-volatile

// **************************************************************************
// config_init
fn_t config_init(void) {
  static bool is_initialized = false;
  int ret;

  // Set initial value
  blink_count_trip = 0U;

  if (false == is_initialized) {
    is_initialized = true;

    // register settings handler
    ret = settings_register(&config_handler);
    if (0 != ret) {
      LOG_ERR("Failed to register settings handler ret:%d", ret);
      return kFailure;
    }

    // load from non-volatile storage
    ret = settings_load_subtree("openblink");
    if (0 != ret) {
      LOG_ERR("Failed to load settings subtree openblink ret:%d", ret);
    }
  }

  return kSuccess;
}

// **************************************************************************
// config_handle_get
int config_handle_get(const char *name, char *val, int val_len_max) {
  const char *next;
  char tmp_32[4];

  LOG_DBG("Config Get: %s", name);

  if (settings_name_steq(name, "blink_count_trip", &next) && !next) {
    memcpy(tmp_32, &blink_count_trip, sizeof(tmp_32));
    val_len_max = MIN(val_len_max, sizeof(tmp_32));
    memcpy(val, tmp_32, val_len_max);
    return val_len_max;
  }
  if (settings_name_steq(name, "blink_count_total", &next) && !next) {
    memcpy(tmp_32, &blink_count_total, sizeof(tmp_32));
    val_len_max = MIN(val_len_max, sizeof(tmp_32));
    memcpy(val, tmp_32, val_len_max);
    return val_len_max;
  }

  return -ENOENT;
}

// **************************************************************************
// config_handle_set
int config_handle_set(const char *name, size_t len, settings_read_cb read_cb,
                      void *cb_arg) {
  const char *next;
  const size_t kNameLen = settings_name_next(name, &next);

  if (!next) {
    LOG_DBG("Config Set: %s", name);

    if (!strncmp(name, "blink_count_trip", kNameLen)) {
      read_cb(cb_arg, &blink_count_trip, sizeof(blink_count_trip));
      return 0;
    }
    if (!strncmp(name, "blink_count_total", kNameLen)) {
      read_cb(cb_arg, &blink_count_total, sizeof(blink_count_total));
      return 0;
    }
  }

  return -ENOENT;
}

// **************************************************************************
// config_handle_export
int config_handle_export(int (*cb)(const char *name, const void *value,
                                   size_t val_len)) {
  LOG_DBG("Config Export");

#if 0
  // blink_count_trip is volatile
  (void)cb("openblink/blink_count_trip", &blink_count_trip, sizeof(blink_count_trip));
#endif
  (void)cb("openblink/blink_count_total", &blink_count_total,
           sizeof(blink_count_total));

  return 0;
}
