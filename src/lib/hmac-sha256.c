/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "hmac-sha256.h"

#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(lib_hmac_sha256, LOG_LEVEL_WRN);

// hmac_sha256_genkey
fn_t hmac_sha256_genkey(hmac_sha256_key_t *const key) {
  psa_status_t status;

  /* Initialize PSA Crypto */
  status = psa_crypto_init();
  if (status != PSA_SUCCESS) {
    return kFailure;
  }

  LOG_INF("Generating random HMAC key...");

  /* Configure the key attributes */
  psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

  psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_VERIFY_HASH |
                                               PSA_KEY_USAGE_SIGN_HASH |
                                               PSA_KEY_USAGE_EXPORT);
  psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
  psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
  psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
  psa_set_key_bits(&key_attributes, 256);

  /* Generate a random key. The key is not exposed to the application,
   * we can use it to encrypt/decrypt using the key handle
   */
  status = psa_generate_key(&key_attributes, &key->id);
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_generate_key failed! (Error: %d)", status);
    return kFailure;
  }

  /* After the key handle is acquired the attributes are not needed */
  psa_reset_key_attributes(&key_attributes);

  LOG_INF("HMAC key generated successfully!");

  /* Export key */
  psa_export_key(key->id, key->value,
                 sizeof(key->value) / sizeof(key->value[0]), &key->len);
  LOG_DBG("key_len: %d", key->len);
  LOG_DBG("key_id: %X", key->id);
  LOG_DBG(
      "key: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
      "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
      "%02X %02X %02X %02X %02X",
      key->value[0], key->value[1], key->value[2], key->value[3], key->value[4],
      key->value[5], key->value[6], key->value[7], key->value[8], key->value[9],
      key->value[10], key->value[11], key->value[12], key->value[13],
      key->value[14], key->value[15], key->value[16], key->value[17],
      key->value[18], key->value[19], key->value[20], key->value[21],
      key->value[22], key->value[23], key->value[24], key->value[25],
      key->value[26], key->value[27], key->value[28], key->value[29],
      key->value[30], key->value[31]);

  return kSuccess;
}

// hmac_sha256_sign
fn_t hmac_sha256_sign(hmac_sha256_key_t *const key,
                      hmac_sha256_hmac_t *const hmac,
                      const uint8_t *const kData, const size_t kDataSize) {
  psa_status_t status;
  psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

  // MAC length should be 32 bytes for 256 - bits key for HMAC - SHA
  if (32 > kDataSize) {
    return kFailure;
  }
  if (sizeof(key->value) / sizeof(key->value[0]) < key->len) {
    return kFailure;
  }

  /* Initialize PSA Crypto */
  status = psa_crypto_init();
  if (status != PSA_SUCCESS) {
    return kFailure;
  }

  /* Import key */
  do {
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(
        &key_attributes, PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&key_attributes, 256);
    psa_import_key(&key_attributes, key->value, key->len, &key->id);
  } while (0);

  LOG_INF("Signing using HMAC ...");

  /* Initialize the HMAC signing operation */
  status =
      psa_mac_sign_setup(&operation, key->id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_sign_setup failed! (Error: %d)", status);
    return kFailure;
  }

  /* Perform the HMAC signing */
  status = psa_mac_update(&operation, kData, kDataSize);
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_update failed! (Error: %d)", status);
    return kFailure;
  }

  /* Finalize the HMAC signing */
  status = psa_mac_sign_finish(&operation, hmac->value,
                               sizeof(hmac->value) / sizeof(hmac->value[0]),
                               &hmac->len);
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_sign_finish failed! (Error: %d)", status);
    return kFailure;
  }

  LOG_INF("Signing successful!");

  LOG_DBG("hmac_len: %d", hmac->len);
  LOG_DBG(
      "HMAC-SHA256: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
      "%02X %02X "
      "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X "
      "%02X %02X %02X %02X %02X",
      hmac->value[0], hmac->value[1], hmac->value[2], hmac->value[3],
      hmac->value[4], hmac->value[5], hmac->value[6], hmac->value[7],
      hmac->value[8], hmac->value[9], hmac->value[10], hmac->value[11],
      hmac->value[12], hmac->value[13], hmac->value[14], hmac->value[15],
      hmac->value[16], hmac->value[17], hmac->value[18], hmac->value[19],
      hmac->value[20], hmac->value[21], hmac->value[22], hmac->value[23],
      hmac->value[24], hmac->value[25], hmac->value[26], hmac->value[27],
      hmac->value[28], hmac->value[29], hmac->value[30], hmac->value[31]);

  return kSuccess;
}

// hmac_sha256_verify
fn_t hmac_sha256_verify(hmac_sha256_key_t *const key,
                        const hmac_sha256_hmac_t *const kHmac,
                        const uint8_t *const kData, const size_t kDataSize) {
  psa_status_t status;
  psa_mac_operation_t operation = PSA_MAC_OPERATION_INIT;

  // MAC length should be 32 bytes for 256 - bits key for HMAC - SHA
  if (32 > kDataSize) {
    return kFailure;
  }
  if (sizeof(key->value) / sizeof(key->value[0]) < key->len) {
    return kFailure;
  }

  /* Initialize PSA Crypto */
  status = psa_crypto_init();
  if (status != PSA_SUCCESS) {
    return kFailure;
  }

  /* Import key */
  do {
    psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_usage_flags(
        &key_attributes, PSA_KEY_USAGE_VERIFY_HASH | PSA_KEY_USAGE_SIGN_HASH);
    psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_HMAC(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_HMAC);
    psa_set_key_bits(&key_attributes, 256);
    psa_import_key(&key_attributes, key->value,
                   sizeof(key->value) / sizeof(key->value[0]), &key->id);
  } while (0);

  /* Initialize the HMAC verification operation */
  status =
      psa_mac_verify_setup(&operation, key->id, PSA_ALG_HMAC(PSA_ALG_SHA_256));
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_verify_setup failed! (Error: %d)", status);
    return kFailure;
  }

  /* Perform the HMAC verification */
  status = psa_mac_update(&operation, kData, kDataSize);
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_update failed! (Error: %d)", status);
    return kFailure;
  }

  /* Finalize the HMAC verification */
  status = psa_mac_verify_finish(
      &operation, kHmac->value, sizeof(kHmac->value) / sizeof(kHmac->value[0]));
  if (status != PSA_SUCCESS) {
    LOG_ERR("psa_mac_verify_finish failed! (Error: %d)", status);
    return kFailure;
  }

  LOG_INF("Verification successful!");

  return kSuccess;
}
