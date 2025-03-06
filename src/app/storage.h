/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef APP_STORAGE_H
#define APP_STORAGE_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../lib/fn.h"

typedef enum {
  kStorageBlinkSlot1 = 1U,
  kStorageBlinkSlot2 = 2U,
} storage_id_t;

fn_t storage_init(void);
ssize_t storage_read_hist(const storage_id_t kId, void *const data,
                          const size_t kLength, const uint16_t kHistoryCounter);
ssize_t storage_read(const storage_id_t kId, void *const data,
                     const size_t kLength);
ssize_t storage_write(const storage_id_t kId, const void *const kData,
                      const size_t kLength);
ssize_t storage_get_data_length(const storage_id_t kId);
int storage_delete(const storage_id_t kId);
ssize_t storage_free_space(void);

#endif  // APP_STORAGE_H
