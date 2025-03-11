/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file storage.h
 * @brief Non-volatile storage management
 * @details Provides functions for reading and writing data to persistent
 * storage
 */
#ifndef APP_STORAGE_H
#define APP_STORAGE_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../lib/fn.h"

/**
 * @typedef storage_id_t
 * @brief Enumeration of storage identifiers
 */
typedef enum {
  kStorageBlinkSlot1 = 1U, /**< Storage ID for first blink slot */
  kStorageBlinkSlot2 = 2U, /**< Storage ID for second blink slot */
} storage_id_t;

/**
 * @brief Initializes the storage subsystem
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t storage_init(void);

/**
 * @brief Reads data from storage with history counter
 *
 * @param kId Storage identifier
 * @param data Buffer to store the read data
 * @param kLength Maximum length of the buffer
 * @param kHistoryCounter History version to read (0 for latest)
 * @return ssize_t The number of bytes read, or negative on error
 */
ssize_t storage_read_hist(const storage_id_t kId, void *const data,
                          const size_t kLength, const uint16_t kHistoryCounter);

/**
 * @brief Reads latest data from storage
 *
 * @param kId Storage identifier
 * @param data Buffer to store the read data
 * @param kLength Maximum length of the buffer
 * @return ssize_t The number of bytes read, or negative on error
 */
ssize_t storage_read(const storage_id_t kId, void *const data,
                     const size_t kLength);

/**
 * @brief Writes data to storage
 *
 * @param kId Storage identifier
 * @param kData Pointer to the data to write
 * @param kLength Length of the data
 * @return ssize_t The number of bytes written, or negative on error
 */
ssize_t storage_write(const storage_id_t kId, const void *const kData,
                      const size_t kLength);

/**
 * @brief Gets the length of data in storage
 *
 * @param kId Storage identifier
 * @return ssize_t The length of the data, or negative on error
 */
ssize_t storage_get_data_length(const storage_id_t kId);

/**
 * @brief Deletes data from storage
 *
 * @param kId Storage identifier
 * @return int 0 on success, negative on error
 */
int storage_delete(const storage_id_t kId);

/**
 * @brief Logs information about free space in storage
 *
 * @return ssize_t Free space in bytes for zms_storage
 */
ssize_t storage_free_space(void);

#endif  // APP_STORAGE_H
