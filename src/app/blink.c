/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.c
 * @brief Implementation of Blink bytecode management
 * @details Implements functions for managing bytecode slots for the mruby/c
 * virtual machine
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

/**
 * @brief Converts a blink slot to a storage ID
 *
 * @param kSlot The blink slot to convert
 * @return storage_id_t The corresponding storage ID
 */
static storage_id_t slot_to_storageid(const blink_slot_t kSlot);

static uint8_t bc_lz4_buf[8000] = {0};

/**
 * @brief Gets the device name with unique identifier
 *
 * @param name Buffer to store the device name
 * @param kBufSize Size of the buffer
 */
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

/**
 * @brief Loads bytecode from the specified slot
 *
 * @param kSlot The slot to load from
 * @param data Buffer to store the bytecode
 * @param kLength Maximum length of the buffer
 * @return ssize_t The number of bytes read, or negative on error
 */
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

/**
 * @brief Stores bytecode to the specified slot
 *
 * @param kSlot The slot to store to
 * @param kData Pointer to the bytecode data
 * @param kLength Length of the bytecode data
 * @return ssize_t The number of bytes written, or negative on error
 */
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

/**
 * @brief Gets the length of bytecode in the specified slot
 *
 * @param kSlot The slot to check
 * @return ssize_t The length of the bytecode, or negative on error
 */
ssize_t blink_get_data_length(const blink_slot_t kSlot) {
  return storage_get_data_length(slot_to_storageid(kSlot));
}

/**
 * @brief Deletes bytecode from the specified slot
 *
 * @param kSlot The slot to delete
 * @return int 0 on success, negative on error
 */
int blink_delete(const blink_slot_t kSlot) {
  return storage_delete(slot_to_storageid(kSlot));
}

/**
 * @brief Converts a blink slot to a storage ID
 *
 * @param kSlot The blink slot to convert
 * @return storage_id_t The corresponding storage ID
 */
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

/**
 * @brief Increments the blink counter values
 *
 * @details Updates both trip and total blink counters in runtime settings
 */
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
