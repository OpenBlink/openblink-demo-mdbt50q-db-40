/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef DRV_BLE_H
#define DRV_BLE_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <zephyr/types.h>

#include "ble_blink.h"

// BLE Callback Event
enum {
  BLE_EVENT_INITIALIZED,
  BLE_EVENT_CONNECTED,
  BLE_EVENT_DISCONNECTED,
  BLE_EVENT_RECEIVED,
  BLE_EVENT_SENT,
  BLE_EVENT_BLINK,
  BLE_EVENT_STATUS,
  BLE_EVENT_REBOOT,
  BLE_EVENT_RELOAD,
};

#pragma pack(1)
// BLE Callback Event Parameter
typedef struct {
  uint8_t event;
  union {
    struct {
    } initialized;
    struct {
    } connected;
    struct {
      uint8_t reason;
    } disconnected;
    struct {
      uint8_t *data;
      size_t size;
    } received;
    struct {
    } sent;
    struct {
      int slot;
      uint8_t *blink_bytecode;
      size_t length;
    } blink;
    struct {
      uint16_t mtu;
    } status;
    struct {
    } reboot;
    struct {
    } reload;
  };
} BLE_PARAM;
#pragma pack()

// BLE Callback function
typedef int (*BLE_CALLBACK)(BLE_PARAM *param);

// BLE Context
typedef struct {
  struct bt_conn *conn;
  BLE_CALLBACK event_cb;
} BLE_CONTEXT;

int ble_init(BLE_CALLBACK cb);
int ble_disconnect();
int ble_start_advertising(const char *local_name);
int ble_stop_advertising();
uint16_t ble_get_mtu();

#endif  // DRV_BLE_H
