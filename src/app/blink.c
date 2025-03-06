/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "blink.h"

#include <zephyr/drivers/hwinfo.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/crc.h>

#include "../lib/fn.h"
#include "lz4.h"
#include "storage.h"

LOG_MODULE_REGISTER(app_blink, LOG_LEVEL_DBG);

static void blink_countup(void);
static storage_id_t slot_to_storageid(const blink_slot_t kSlot);
static uint8_t bc_lz4_buf[8000] = {0};

// **************************************************************************//
// blink_get_name
void blink_get_name(char *const name, const size_t kBufSize) {
  char tmp_name[BLINK_DEVICE_NAME_SIZE] = {0};
  char dev_id[4 + 1] = {0};
  uint8_t buf[8] = {0x00U};
  hwinfo_get_device_id(buf, (sizeof(buf) / sizeof(buf[0])));
  snprintf(dev_id, sizeof(dev_id), "%04X",
           crc16_reflect(0x9eb2U, 0xFFFFU, buf, sizeof(buf)));

  strcpy(tmp_name, CONFIG_BT_DEVICE_NAME);
  strcat(tmp_name, "_");
  strcat(tmp_name, dev_id);
  strncpy(name, tmp_name, MIN(strlen(tmp_name), kBufSize - 1));

  return;
}

// **************************************************************************//
// blink_load
ssize_t blink_load(const blink_slot_t kSlot, void *const data,
                   const size_t kLength) {
  int rc = (int)storage_read(slot_to_storageid(kSlot), bc_lz4_buf,
                             sizeof(bc_lz4_buf));
  if (0 > rc) {
    LOG_ERR("storage_read failed");
    return rc;
  }
  rc = LZ4_decompress_safe(bc_lz4_buf, data, rc, kLength);
  if (0 > rc) {
    LOG_ERR("LZ4_decompress_safe failed");
    return rc;
  }

  return rc;
}

// **************************************************************************
// blink_store
ssize_t blink_store(const blink_slot_t kSlot, const void *const kData,
                    const size_t kLength) {
  const int kRc = LZ4_compress_default(kData, bc_lz4_buf, (int)kLength,
                                       (int)sizeof(bc_lz4_buf));
  if (0 > kRc) {
    LOG_ERR("LZ4_compress_default failed");
    return kRc;
  }
  blink_countup();
  return storage_write(slot_to_storageid(kSlot), bc_lz4_buf, kRc);
}

// **************************************************************************
// blink_get_data_length
ssize_t blink_get_data_length(const blink_slot_t kSlot) {
  return storage_get_data_length(slot_to_storageid(kSlot));
}

// **************************************************************************
// blink_delete
int blink_delete(const blink_slot_t kSlot) {
  return storage_delete(slot_to_storageid(kSlot));
}

// **************************************************************************
// slot_to_storageid
static storage_id_t slot_to_storageid(const blink_slot_t kSlot) {
  switch (kSlot) {
    case kBlinkSlot1:
      return kStorageBlinkSlot1;
      break;
    case kBlinkSlot2:
      return kStorageBlinkSlot2;
      break;
    default:
      return kStorageBlinkSlot1;
      break;
  }
}

// **************************************************************************
// blink_countup
static void blink_countup(void) {
  uint32_t tmp_trip = 0U;
  uint32_t tmp_total = 0U;
  settings_runtime_get("openblink/blink_count_trip", &tmp_trip,
                       sizeof(tmp_trip));
  settings_runtime_get("openblink/blink_count_total", &tmp_total,
                       sizeof(tmp_total));

  tmp_trip += (UINT32_MAX > tmp_trip) ? 1 : 0;
  tmp_total += (UINT32_MAX > tmp_total) ? 1 : 0;

  settings_runtime_set("openblink/blink_count_trip", &tmp_trip,
                       sizeof(tmp_trip));
  settings_runtime_set("openblink/blink_count_total", &tmp_total,
                       sizeof(tmp_total));

  LOG_DBG("blink_count_trip: %u, blink_count_total: %u", tmp_trip, tmp_total);
}
