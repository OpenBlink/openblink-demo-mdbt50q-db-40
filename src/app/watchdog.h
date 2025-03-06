/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef APP_WATCHDOG_H
#define APP_WATCHDOG_H

#include <stdint.h>

#include "../lib/fn.h"

typedef enum {
  kAppWatchDogThreadMain,
  kAppWatchDogThreadSysWorkQ,
  kAppWatchDogThreadNum,  // Number of threads
} app_watchdog_thread_t;

fn_t watchdog_init(void);
uint32_t watchdog_get_max_window_ms(void);
fn_t watchdog_thread_hearbeat(const app_watchdog_thread_t kThreadName);

#endif  // APP_WATCHDOG_H
