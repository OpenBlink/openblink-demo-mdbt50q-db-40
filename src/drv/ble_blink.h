/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef DRV_BLE_BLINK_H
#define DRV_BLE_BLINK_H

#include <stdint.h>
#include <zephyr/bluetooth/uuid.h>

// Service: 227da52c-e13a-412b-befb-ba2256bb7fbe
#define OPENBLINK_SERVICE_UUID \
  BT_UUID_128_ENCODE(0x227da52c, 0xe13a, 0x412b, 0xbefb, 0xba2256bb7fbe)

int ble_blink_init();
int ble_print(const char *data);

#endif  // DRV_BLE_BLINK_H
