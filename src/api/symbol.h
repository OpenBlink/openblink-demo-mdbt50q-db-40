/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef API_SYMBOL_H
#define API_SYMBOL_H

#include <stdint.h>

#include "../lib/fn.h"

typedef enum {
  kSymbolLED1,
  kSymbolLED2,
  kSymbolLED3,
  kSymbolSW1,
  kSymbolSW2,
  kSymbolSW3,
  kSymbolSW4,
  kSymbolTSize  // enum size
} symbol_t;

fn_t api_symbol_init(void);
fn_t api_symbol_define(void);
int16_t api_symbol_get_id(const symbol_t kSymbol);

#endif
