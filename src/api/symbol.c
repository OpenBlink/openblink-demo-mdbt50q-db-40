/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "symbol.h"

#include <stddef.h>
#include <stdint.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

static int16_t symbol_id_table[kSymbolTSize];
static void symbol_regist(const char *const kName, const symbol_t kEnum);

// **************************************************************************
// api_symbol_init
fn_t api_symbol_init(void) {
  for (size_t i = 0; i < kSymbolTSize; i++) {
    symbol_id_table[i] = -1;
  }
  return kSuccess;
}

// **************************************************************************
// api_symbol_define
fn_t api_symbol_define(void) {
  symbol_regist("led1", kSymbolLED1);
  symbol_regist("led2", kSymbolLED2);
  symbol_regist("led3", kSymbolLED3);
  symbol_regist("sw1", kSymbolSW1);
  symbol_regist("sw2", kSymbolSW2);
  symbol_regist("sw3", kSymbolSW3);
  symbol_regist("sw4", kSymbolSW4);
  for (size_t i = 0; i < kSymbolTSize; i++) {
    if (-1 == symbol_id_table[i]) {
      return kFailure;
    }
  }
  return kSuccess;
}

// **************************************************************************
// api_symbol_get_id
int16_t api_symbol_get_id(const symbol_t kSymbol) {
  if (kSymbolTSize <= kSymbol) {
    return -1;
  } else {
    return (symbol_id_table[kSymbol]);
  }
}

// **************************************************************************
// symbol_regist
static void symbol_regist(const char *const kName, const symbol_t kEnum) {
  mrbc_symbol_new(0, kName);
  symbol_id_table[kEnum] = (int16_t)mrbc_search_symid(kName);
}
