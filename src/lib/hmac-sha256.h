/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#ifndef LIB_HMAC_H
#define LIB_HMAC_H

#include <psa/crypto_types.h>
#include <stddef.h>
#include <stdint.h>

#include "fn.h"

typedef struct {
  uint8_t value[32];
  psa_key_id_t id;
  size_t len;
} hmac_sha256_key_t;

typedef struct {
  uint8_t value[32];
  size_t len;
} hmac_sha256_hmac_t;

fn_t hmac_sha256_genkey(hmac_sha256_key_t *const key);

fn_t hmac_sha256_sign(hmac_sha256_key_t *const key,
                      hmac_sha256_hmac_t *const hmac,
                      const uint8_t *const kData, const size_t kDataSize);

fn_t hmac_sha256_verify(hmac_sha256_key_t *const key,
                        const hmac_sha256_hmac_t *const kHmac,
                        const uint8_t *const kData, const size_t kDataSize);

#endif  // LIB_HMAC_H
