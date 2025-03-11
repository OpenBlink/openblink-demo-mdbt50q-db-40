/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file hmac-sha256.h
 * @brief HMAC-SHA256 cryptographic functions interface
 * @details Provides functions for key generation, signing and verification
 * using HMAC-SHA256
 */
#ifndef LIB_HMAC_H
#define LIB_HMAC_H

#include <psa/crypto_types.h>
#include <stddef.h>
#include <stdint.h>

#include "fn.h"

/**
 * @typedef hmac_sha256_key_t
 * @brief Structure for HMAC-SHA256 key storage
 */
typedef struct {
  uint8_t value[32]; /**< Key value buffer */
  psa_key_id_t id;   /**< PSA key identifier */
  size_t len;        /**< Key length */
} hmac_sha256_key_t;

/**
 * @typedef hmac_sha256_hmac_t
 * @brief Structure for HMAC-SHA256 hash value storage
 */
typedef struct {
  uint8_t value[32]; /**< HMAC value buffer */
  size_t len;        /**< HMAC length */
} hmac_sha256_hmac_t;

/**
 * @brief Generates a new HMAC-SHA256 key
 *
 * @param key Pointer to key structure to store the generated key
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t hmac_sha256_genkey(hmac_sha256_key_t *const key);

/**
 * @brief Signs data using HMAC-SHA256
 *
 * @param key Pointer to the key structure
 * @param hmac Pointer to store the resulting HMAC
 * @param kData Pointer to the data to sign
 * @param kDataSize Size of the data in bytes
 * @return fn_t kSuccess if successful, kFailure otherwise
 */
fn_t hmac_sha256_sign(hmac_sha256_key_t *const key,
                      hmac_sha256_hmac_t *const hmac,
                      const uint8_t *const kData, const size_t kDataSize);

/**
 * @brief Verifies data using HMAC-SHA256
 *
 * @param key Pointer to the key structure
 * @param kHmac Pointer to the HMAC to verify against
 * @param kData Pointer to the data to verify
 * @param kDataSize Size of the data in bytes
 * @return fn_t kSuccess if verification successful, kFailure otherwise
 */
fn_t hmac_sha256_verify(hmac_sha256_key_t *const key,
                        const hmac_sha256_hmac_t *const kHmac,
                        const uint8_t *const kData, const size_t kDataSize);

#endif  // LIB_HMAC_H
