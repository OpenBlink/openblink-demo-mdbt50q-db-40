/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef APP_BLINK_H
#define APP_BLINK_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#define BLINK_MAX_BYTECODE_SIZE (8000 * 2)
#define BLINK_DEVICE_NAME_SIZE (sizeof(CONFIG_BT_DEVICE_NAME) + 1 + 4 + 1)

typedef enum {
  kBlinkSlot1 = 1U,
  kBlinkSlot2 = 2U,
} blink_slot_t;

void blink_get_name(char *const name, const size_t kBufSize);
ssize_t blink_load(const blink_slot_t kSlot, void *const data,
                   const size_t kLength);
ssize_t blink_store(const blink_slot_t kSlot, const void *const kData,
                    const size_t kLength);
ssize_t blink_get_data_length(const blink_slot_t kSlot);
int blink_delete(const blink_slot_t kSlot);

#endif
