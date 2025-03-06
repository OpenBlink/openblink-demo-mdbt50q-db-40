/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "storage.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(app_storage, LOG_LEVEL_DBG);
K_MUTEX_DEFINE(mutex_storage);

#define NVS_PARTITION nvs_storage
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

static struct nvs_fs fs;

// **************************************************************************
// storage_init
fn_t storage_init(void) {
  struct flash_pages_info info;
  int rc = 0;

  fs.flash_device = NVS_PARTITION_DEVICE;
  if (!device_is_ready(fs.flash_device)) {
    LOG_ERR("Storage device %s is not ready", fs.flash_device->name);
    return kFailure;
  }

  fs.offset = NVS_PARTITION_OFFSET;
  rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
  if (rc) {
    LOG_ERR("Unable to get page info, rc=%d", rc);
    return kFailure;
  }

  fs.sector_size = info.size * 2U;
  fs.sector_count = 30U / 2U;

  rc = nvs_mount(&fs);
  if (rc) {
    LOG_ERR("Storage Init failed, rc=%d", rc);
    return kFailure;
  }

  return kSuccess;
}

// **************************************************************************
// storage_read_hist
ssize_t storage_read_hist(const storage_id_t kId, void *const data,
                          const size_t kLength,
                          const uint16_t kHistoryCounter) {
  ssize_t kRc =
      nvs_read_hist(&fs, (uint32_t)kId, data, kLength, kHistoryCounter);
  LOG_DBG("storage_read_hist ID:%d, Length:%d, Cnt:%d, Return:%d", kId, kLength,
          kHistoryCounter, kRc);
  return kRc;
}

// **************************************************************************
// storage_read
ssize_t storage_read(const storage_id_t kId, void *const data,
                     const size_t kLength) {
  return storage_read_hist(kId, data, kLength, 0);
}

// **************************************************************************
// storage_write
ssize_t storage_write(const storage_id_t kId, const void *const kData,
                      const size_t kLength) {
  k_mutex_lock(&mutex_storage, K_FOREVER);
  ssize_t kRc = nvs_write(&fs, (uint32_t)kId, kData, kLength);
  k_mutex_unlock(&mutex_storage);
  LOG_DBG("storage_write ID:%d, Length:%d, Return:%d", kId, kLength, kRc);
  return kRc;
}

// **************************************************************************
// storage_get_data_length
ssize_t storage_get_data_length(const storage_id_t kId) {
  return nvs_read(&fs, (uint32_t)kId, NULL, 0);
}

// **************************************************************************
// storage_delete
int storage_delete(const storage_id_t kId) {
  k_mutex_lock(&mutex_storage, K_FOREVER);
  int kRc = nvs_delete(&fs, (uint32_t)kId);
  k_mutex_unlock(&mutex_storage);
  LOG_DBG("storage_delete ID:%d, Return:%d", kId, kRc);
  return kRc;
}

// **************************************************************************
// storage_free_space
ssize_t storage_free_space(void) {
  // nvs_storage
  const ssize_t kRc = nvs_calc_free_space(&fs);
  LOG_INF("nvs_storage: %zu bytes free", kRc);

  // settings_storage
  struct nvs_fs *fs_settings;
  const int kErr = settings_storage_get((void **)&fs_settings);
  if (0 > kErr) {
    LOG_ERR("settings_storage_get failed");
  } else {
    LOG_INF("settings_storage: %zu bytes free",
            nvs_calc_free_space(fs_settings));
  }

  return kRc;  // nvs_storage
}
