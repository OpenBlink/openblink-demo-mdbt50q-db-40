/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef APP_COMM_H
#define APP_COMM_H

#include <stdbool.h>

#include "../lib/fn.h"

fn_t comm_init(void);
bool comm_get_advertising(void);
bool comm_get_connected(void);
fn_t comm_disconnect(void);

#endif  // APP_COMM_H
