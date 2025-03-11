/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file symbol.c
 * @brief Implementation of symbol management for mruby/c
 * @details Implements the symbol registration and lookup for mruby/c scripts
 */
#include "symbol.h"

#include <stddef.h>
#include <stdint.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

/**
 * @brief Table to store symbol IDs for each symbol in the enum
 */
static int16_t symbol_id_table[kSymbolTSize];

/**
 * @brief Registers a symbol with mruby/c and stores its ID
 *
 * @param kName The name of the symbol to register
 * @param kEnum The enum value to associate with the symbol
 */
static void symbol_regist(const char *const kName, const symbol_t kEnum);

/**
 * @brief Initializes the symbol table for mruby/c
 *
 * @details Sets all symbol IDs to -1 (invalid) to prepare for registration
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_symbol_init(void) {
  for (size_t i = 0; i < kSymbolTSize; i++) {
    symbol_id_table[i] = -1;
  }
  return kSuccess;
}

/**
 * @brief Defines all symbols in the mruby/c environment
 *
 * @details Registers all LED and switch symbols with mruby/c and verifies
 *          that all symbols were registered successfully
 *
 * @return fn_t kSuccess if all symbols were registered successfully, kFailure
 * otherwise
 */
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

/**
 * @brief Gets the mruby/c symbol ID for a given symbol
 *
 * @param kSymbol The symbol to get the ID for
 * @return int16_t The mruby/c symbol ID, or -1 if invalid
 */
int16_t api_symbol_get_id(const symbol_t kSymbol) {
  if (kSymbolTSize <= kSymbol) {
    return -1;
  } else {
    return (symbol_id_table[kSymbol]);
  }
}

/**
 * @brief Registers a symbol with mruby/c and stores its ID
 *
 * @param kName The name of the symbol to register
 * @param kEnum The enum value to associate with the symbol
 */
static void symbol_regist(const char *const kName, const symbol_t kEnum) {
  mrbc_symbol_new(0, kName);
  symbol_id_table[kEnum] = (int16_t)mrbc_search_symid(kName);
}
