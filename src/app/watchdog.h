/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file watchdog.h
 * @brief Watchdog timer interface
 * @details Provides functions for managing the system watchdog timer
 */
#ifndef APP_WATCHDOG_H
#define APP_WATCHDOG_H

#include <stdint.h>

#include "../lib/fn.h"

/**
 * @typedef app_watchdog_thread_t
 * @brief Enumeration of threads monitored by the watchdog
 */
typedef enum {
  kAppWatchDogThreadMain,     /**< Main thread */
  kAppWatchDogThreadSysWorkQ, /**< System work queue thread */
  kAppWatchDogThreadNum,      /**< Number of threads (not a thread) */
} app_watchdog_thread_t;

/**
 * @brief Initializes the watchdog timer
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t watchdog_init(void);

/**
 * @brief Gets the maximum watchdog window in milliseconds
 *
 * @return uint32_t Maximum watchdog window in milliseconds
 */
uint32_t watchdog_get_max_window_ms(void);

/**
 * @brief Sends a heartbeat for the specified thread
 *
 * @param kThreadName The thread to send a heartbeat for
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t watchdog_thread_hearbeat(const app_watchdog_thread_t kThreadName);

#endif  // APP_WATCHDOG_H
