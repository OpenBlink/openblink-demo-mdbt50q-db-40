/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "ble_blink.h"

#include <assert.h>
#include <errno.h>
#include <soc.h>
#include <stddef.h>
#include <string.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/crc.h>
#include <zephyr/sys/util.h>
#include <zephyr/types.h>

#include "../app/blink.h"
#include "ble.h"

LOG_MODULE_REGISTER(ble_blink, LOG_LEVEL_DBG);

// Service: 227da52c-e13a-412b-befb-ba2256bb7fbe
#define BT_UUID_OPENBLINK_SERVICE BT_UUID_DECLARE_128(OPENBLINK_SERVICE_UUID)

// Program: ad9fdd56-1135-4a84-923c-ce5a244385e7
#define OPEN_BLINK_PROGRAM_CHARACTERISTIC_UUID \
  BT_UUID_128_ENCODE(0xad9fdd56, 0x1135, 0x4a84, 0x923c, 0xce5a244385e7)
#define BT_UUID_OPEN_BLINK_PROGRAM_CHARACTERISTIC_UUID \
  BT_UUID_DECLARE_128(OPEN_BLINK_PROGRAM_CHARACTERISTIC_UUID)

// Console: a015b3de-185a-4252-aa04-7a87d38ce148
#define OPEN_BLINK_CONSOLE_CHARACTERISTIC_UUID \
  BT_UUID_128_ENCODE(0xa015b3de, 0x185a, 0x4252, 0xaa04, 0x7a87d38ce148)
#define BT_UUID_OPEN_BLINK_CONSOLE_CHARACTERISTIC_UUID \
  BT_UUID_DECLARE_128(OPEN_BLINK_CONSOLE_CHARACTERISTIC_UUID)

// Status: ca141151-3113-448b-b21a-6a6203d253ff
#define OPEN_BLINK_STATUS_CHARACTERISTIC_UUID \
  BT_UUID_128_ENCODE(0xca141151, 0x3113, 0x448b, 0xb21a, 0x6a6203d253ff)
#define BT_UUID_OPEN_BLINK_STATUS_CHARACTERISTIC_UUID \
  BT_UUID_DECLARE_128(OPEN_BLINK_STATUS_CHARACTERISTIC_UUID)

#define BLINK_VERSION 0x01

#define BLINK_CMD_DATA 'D'    // Data
#define BLINK_CMD_PROG 'P'    // Program
#define BLINK_CMD_RESET 'R'   // softReset
#define BLINK_CMD_RELOAD 'L'  // reLoad

#pragma pack(1)
typedef struct {
  uint8_t version;     // [1] Blink Version (0x01)
  uint8_t command;     // [1] 'D':Data 'P':Program 'R':SoftReset
} BLINK_CHUNK_HEADER;  // 2bytes
#pragma pack()

#pragma pack(1)
typedef struct {
  BLINK_CHUNK_HEADER header;  // [2] Header
  uint16_t offset;            // [2]
  uint16_t size;              // [2]
} BLINK_CHUNK_DATA;           // 6bytes
#pragma pack()

#pragma pack(1)
typedef struct {
  BLINK_CHUNK_HEADER header;  // [2] Header
  uint16_t length;            // [2]
  uint16_t crc;               // [2] CRC16
  uint8_t slot;               // [1]
  uint8_t reserved;           // [1]
} BLINK_CHUNK_PROGRAM;        // 6bytes
#pragma pack()

// -------------------------------------------------------------------------------------------

extern BLE_CONTEXT ble_context;

static uint8_t blink_bytecode[BLINK_MAX_BYTECODE_SIZE] = {0};

static int notify_blink_program(const char *data);

// **************************************************************************
// blink_result_error
static void blink_result_error(const char *msg) {
  LOG_ERR("%s", msg);
  notify_blink_program(msg);
}

// **************************************************************************
// Blink Command: 'D'ata
static int blink_program_command_D(BLINK_CHUNK_HEADER *header, uint16_t len) {
  BLINK_CHUNK_DATA *data_chunk = (BLINK_CHUNK_DATA *)header;

  const int size = data_chunk->size;
  const int offset = data_chunk->offset;

  if (sizeof(BLINK_CHUNK_DATA) + size != len) {
    LOG_ERR("%d / %d", sizeof(BLINK_CHUNK_DATA) + size, len);
    blink_result_error("ERROR: Blink data size error");
    return -1;
  }

  LOG_DBG(
      "BLE: Blink 'D'ata chunk:"
      "len:%d size:%d offset:%d",
      len, size, offset);

  if (offset + size > BLINK_MAX_BYTECODE_SIZE) {
    blink_result_error("ERROR: Size exceeds buffer limits");
    return -1;
  }

  uint8_t *buffer = (uint8_t *)(data_chunk + 1);
  memcpy(&blink_bytecode[offset], buffer, size);
  return 0;
}

// **************************************************************************
// Blink Command: 'P'rogram
static int blink_program_command_P(BLINK_CHUNK_HEADER *header) {
  BLINK_CHUNK_PROGRAM *p = (BLINK_CHUNK_PROGRAM *)header;

  LOG_DBG("BLE: Blink 'P'rogram size:%d slot:%d CRC16:0x%08X", p->length,
          p->slot, p->crc);

  if (p->length > BLINK_MAX_BYTECODE_SIZE) {
    blink_result_error("ERROR: Size exceeds buffer limits");
    return -EINVAL;
  }

  // CRC16
  uint16_t crc16 = crc16_reflect(0xd175U, 0xFFFFU, blink_bytecode, p->length);
  LOG_DBG("BLE: Blink CRC16: 0x%08X == 0x%08X", crc16, p->crc);

  if (crc16 == p->crc) {
    BLE_PARAM param = {
        .event = BLE_EVENT_BLINK,
        .blink.blink_bytecode = &blink_bytecode[0],
        .blink.slot = p->slot,
        .blink.length = p->length,
    };

    int err = ble_context.event_cb(&param);
    if (err == 0) {
      LOG_DBG("blink_bytecode:%d", p->length);
      char str[64];
      snprintf(str, sizeof(str), "OK slot:%d", p->slot);
      notify_blink_program(str);
    } else {
      blink_result_error("ERROR: Blink program error");
    }

  } else {
    blink_result_error("ERROR: CRC mismatch");
  }

  // Clear the buffer
  memset(&blink_bytecode, 0, sizeof(blink_bytecode));
  return 0;
}

// **************************************************************************
// blink_write_program
static ssize_t blink_write_program(struct bt_conn *conn,
                                   const struct bt_gatt_attr *attr,
                                   const void *buf, uint16_t len,
                                   uint16_t offset, uint8_t flags) {
  ARG_UNUSED(attr);
  ARG_UNUSED(offset);
  ARG_UNUSED(flags);

  BLINK_CHUNK_HEADER *header = (BLINK_CHUNK_HEADER *)buf;
  LOG_DBG("BLE: Blink Command [%c]", header->command);

  // Check the version
  if (header->version != BLINK_VERSION) {
    blink_result_error("ERROR: Blink version mismatch");
  }

  switch (header->command) {
    case BLINK_CMD_DATA:
      blink_program_command_D(header, len);
      break;
    case BLINK_CMD_PROG:
      if (sizeof(BLINK_CHUNK_PROGRAM) != len) {
        blink_result_error("ERROR: Blink size mismatch");
      } else {
        blink_program_command_P(header);
      }
      break;
    case BLINK_CMD_RESET:
      BLE_PARAM param_reset = {
          .event = BLE_EVENT_REBOOT,
      };
      ble_context.event_cb(&param_reset);
      break;
    case BLINK_CMD_RELOAD:
      BLE_PARAM param_reload = {
          .event = BLE_EVENT_RELOAD,
      };
      ble_context.event_cb(&param_reload);
      break;
    default:
      blink_result_error("ERROR: Blink unknown type");
  }

  return len;
}

// **************************************************************************
// blink_read_mtu
static ssize_t blink_read_mtu(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr, void *buf,
                              uint16_t len, uint16_t offset) {
  BLE_PARAM param = {
      .event = BLE_EVENT_STATUS,
      .status.mtu = bt_gatt_get_mtu(conn),
  };
  ble_context.event_cb(&param);

  return bt_gatt_attr_read(conn, attr, buf, len, offset, &param.status.mtu,
                           sizeof(param.status.mtu));
}

// **************************************************************************
// on_cccd_changed
static void on_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value) {
  LOG_DBG("BLE: CCCD changed to %04x", value);
}

// **************************************************************************
// BLE GATT Attributes
static struct bt_gatt_attr attrs[] = {
    // 0
    BT_GATT_PRIMARY_SERVICE(BT_UUID_OPENBLINK_SERVICE),
    // Console: 1, [2], 3
    BT_GATT_CHARACTERISTIC(BT_UUID_OPEN_BLINK_CONSOLE_CHARACTERISTIC_UUID,
                           BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL,
                           NULL),
    BT_GATT_CCC(on_cccd_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    // Program: 4, [5]
    BT_GATT_CHARACTERISTIC(BT_UUID_OPEN_BLINK_PROGRAM_CHARACTERISTIC_UUID,
                           BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_WRITE, NULL, blink_write_program, NULL),
    // MTU: 6, [7]
    BT_GATT_CHARACTERISTIC(BT_UUID_OPEN_BLINK_STATUS_CHARACTERISTIC_UUID,
                           BT_GATT_CHRC_READ, BT_GATT_PERM_READ, blink_read_mtu,
                           NULL, NULL),
};

#define SERVICE_BLINK_CONSOLE 2  // [2]
#define SERVICE_BLINK_PROGRAM 5  // [5]
#define SERVICE_BLINK_STATUS 7   // [7]

static struct bt_gatt_service service = BT_GATT_SERVICE(attrs);

// **************************************************************************
// notify_blink_program
static int notify_blink_program(const char *data) {
  int err = 0;
  const struct bt_gatt_attr *attr = &attrs[SERVICE_BLINK_PROGRAM];

  if (bt_gatt_is_subscribed(ble_context.conn, attr, BT_GATT_CCC_NOTIFY)) {
    size_t size = strlen(data);

    err = bt_gatt_notify(ble_context.conn, attr, data, size);
    if (err) {
      LOG_ERR("BLE: unable to send notification");
    }
  }
  return err;
}

// **************************************************************************
// ble_blink_init
int ble_blink_init() {
  int err = bt_gatt_service_register(&service);
  if (err) {
    LOG_ERR("BLE: Blink service register error %d", err);
  }
  return err;
}

// **************************************************************************
// ble_print
int ble_print(const char *data) {
  int err = 0;
  const struct bt_gatt_attr *attr = &attrs[SERVICE_BLINK_CONSOLE];

  if (bt_gatt_is_subscribed(ble_context.conn, attr, BT_GATT_CCC_NOTIFY)) {
    size_t size = strlen(data);

    err = bt_gatt_notify(ble_context.conn, attr, data, size);
    if (err) {
      LOG_ERR("BLE: unable to send notification");
    }
  }

  return err;
}
