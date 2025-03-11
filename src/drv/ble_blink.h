/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file ble_blink.h
 * @brief Bluetooth Low Energy Blink service interface
 * @details Defines the Blink service UUID and initialization functions
 */
#ifndef DRV_BLE_BLINK_H
#define DRV_BLE_BLINK_H

#include <stdint.h>
#include <zephyr/bluetooth/uuid.h>

/**
 * @brief UUID for the OpenBlink service
 * @details 128-bit UUID used to identify the OpenBlink service in BLE
 * advertisements
 */
#define OPENBLINK_SERVICE_UUID \
  BT_UUID_128_ENCODE(0x227da52c, 0xe13a, 0x412b, 0xbefb, 0xba2256bb7fbe)

/**
 * @brief Initializes the BLE Blink service
 *
 * @return int 0 on success, negative on error
 */
int ble_blink_init();

/**
 * @brief Sends a string over BLE console characteristic
 *
 * @param data String to send
 * @return int 0 on success, negative on error
 */
int ble_print(const char *data);

#endif  // DRV_BLE_BLINK_H
