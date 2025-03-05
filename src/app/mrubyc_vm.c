/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "mrubyc_vm.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../api/api.h"
#include "../api/ble.h"
#include "../api/blink.h"
#include "../api/input.h"
#include "../api/led.h"
#include "../api/symbol.h"
#include "../drv/ble.h"
#include "../lib/fn.h"
#include "../rb/slot1.h"
#include "../rb/slot2.h"
#include "blink.h"
#include "init.h"

LOG_MODULE_REGISTER(app_mrubyc_vm, LOG_LEVEL_DBG);

#define MRBC_HEAP_MEMORY_SIZE (15 * 1024)
#define MRUBYC_VM_MAIN_STACK_SIZE (50 * 1024)

static bool request_mruby_reload = false;
static void load_bytecode(const blink_slot_t kSlot, uint8_t *const bytecode,
                          const size_t kLength);
static void mrubyc_vm_main(void *, void *, void *);
static void mrubyc_timerhandler(struct k_timer *const timer) { mrbc_tick(); }
K_THREAD_DEFINE(th_mrubyc_vm_main, MRUBYC_VM_MAIN_STACK_SIZE, mrubyc_vm_main,
                NULL, NULL, NULL, 1, 0, 1);
K_TIMER_DEFINE(timer_mrubyc, mrubyc_timerhandler, NULL);

// **************************************************************************
// app_mrubyc_vm_set_reload
fn_t app_mrubyc_vm_set_reload(void) {
  request_mruby_reload = true;
  return kSuccess;
}

// **************************************************************************
// app_mrubyc_vm_get_reload
bool app_mrubyc_vm_get_reload(void) { return request_mruby_reload; }

// **************************************************************************
// mrubyc_vm_main
static void mrubyc_vm_main(void *, void *, void *) {
  int64_t timestamp = k_uptime_get();
  char buf_blink_time[100] = {0};

  while (1) {
    mrbc_tcb *tcb[MAX_VM_COUNT] = {NULL};
    uint8_t memory_pool[MRBC_HEAP_MEMORY_SIZE] = {0};
    uint8_t bytecode_slot1[BLINK_MAX_BYTECODE_SIZE] = {0};
    uint8_t bytecode_slot2[BLINK_MAX_BYTECODE_SIZE] = {0};

    // mruby/c initialize
    mrbc_init(memory_pool, MRBC_HEAP_MEMORY_SIZE);

    ////////////////////
    // Symbol
    if (kSuccess != api_symbol_define()) {
      LOG_ERR("Failed to define symbol");
    }
    // Class, Method
    api_led_define();    // LED.*
    api_input_define();  // Input.*
    api_ble_define();    // BLE.*
    api_blink_define();  // Blink.*

    ////////////////////
    // Clear reload request flag
    request_mruby_reload = false;
    // Load mruby bytecode
    load_bytecode(kBlinkSlot1, bytecode_slot1,
                  sizeof(bytecode_slot1) / sizeof(bytecode_slot1[0]));
    load_bytecode(kBlinkSlot2, bytecode_slot2,
                  sizeof(bytecode_slot2) / sizeof(bytecode_slot2[0]));

    ////////////////////
    // mruby/c create task
    tcb[0] = mrbc_create_task(bytecode_slot1, NULL);
    tcb[1] = mrbc_create_task(bytecode_slot2, NULL);
    if ((tcb[0] == NULL) || (tcb[1] == NULL)) {
      LOG_ERR("Failed to create task");
    }
    // set priority
    mrbc_change_priority(tcb[0], 1);
    mrbc_change_priority(tcb[1], 2);

    ////////////////////
    snprintf(buf_blink_time, sizeof(buf_blink_time), "Blinked (%lli ms)\n",
             k_uptime_delta(&timestamp));
    ble_print(buf_blink_time);

    k_timer_start(&timer_mrubyc, K_NO_WAIT, K_MSEC(1));
    mrbc_run();
    k_timer_stop(&timer_mrubyc);

    snprintf(buf_blink_time, sizeof(buf_blink_time),
             "mrbc_run Stopped (uptime: %lli ms)\n",
             k_uptime_delta(&timestamp));
    ble_print(buf_blink_time);

    ////////////////////
    // mruby/c cleanup
    mrbc_cleanup();
  }
}

// **************************************************************************
// load_bytecode
static void load_bytecode(const blink_slot_t kSlot, uint8_t *const bytecode,
                          const size_t kLength) {
  ssize_t rc = 0;
  rc = blink_get_data_length(kSlot);
  if ((0 < rc) && (kLength >= rc)) {
    // Load from non-volatile memory
    rc = blink_load(kSlot, bytecode, kLength);
    if (0 < rc) {
      LOG_DBG("Slot:%d, Size:%d/%d", kSlot, rc, kLength);
      return;
    }
  } else {
    // Load from factory default program
    switch (kSlot) {
      case kBlinkSlot1:
        rc = (ssize_t)sizeof(slot1);
        if (rc <= (ssize_t)kLength) {
          memcpy(bytecode, &slot1[0], (size_t)rc);
        }
        break;
      case kBlinkSlot2:
        rc = (ssize_t)sizeof(slot2);
        if (rc <= (ssize_t)kLength) {
          memcpy(bytecode, &slot2[0], (size_t)rc);
        }
        break;
      default:
        break;
    }
    if (0 < rc) {
      LOG_DBG("Slot:%d, Size:%d/%d, Factory default program loaded.", kSlot, rc,
              kLength);
    }
  }
}
