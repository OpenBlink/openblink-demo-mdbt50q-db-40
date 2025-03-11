/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file symbol.h
 * @brief Symbol definitions for mruby/c
 * @details Defines symbols used for LED and button identifiers in mruby/c
 * scripts
 */
#ifndef API_SYMBOL_H
#define API_SYMBOL_H

#include <stdint.h>

#include "../lib/fn.h"

/**
 * @typedef symbol_t
 * @brief Enumeration of symbols used in the API
 */
typedef enum {
  kSymbolLED1, /**< Symbol for LED1 */
  kSymbolLED2, /**< Symbol for LED2 */
  kSymbolLED3, /**< Symbol for LED3 */
  kSymbolSW1,  /**< Symbol for switch/button 1 */
  kSymbolSW2,  /**< Symbol for switch/button 2 */
  kSymbolSW3,  /**< Symbol for switch/button 3 */
  kSymbolSW4,  /**< Symbol for switch/button 4 */
  kSymbolTSize /**< Total number of symbols (enum size) */
} symbol_t;

/**
 * @brief Initializes the symbol table for mruby/c
 *
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t api_symbol_init(void);

/**
 * @brief Defines all symbols in the mruby/c environment
 *
 * @return fn_t kSuccess if all symbols were registered successfully, kFailure
 * otherwise
 */
fn_t api_symbol_define(void);

/**
 * @brief Gets the mruby/c symbol ID for a given symbol
 *
 * @param kSymbol The symbol to get the ID for
 * @return int16_t The mruby/c symbol ID, or -1 if invalid
 */
int16_t api_symbol_get_id(const symbol_t kSymbol);

#endif
