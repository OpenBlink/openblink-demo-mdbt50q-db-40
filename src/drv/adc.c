/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
#include "adc.h"

#include <stdbool.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../lib/fn.h"

LOG_MODULE_REGISTER(drv_adc, LOG_LEVEL_DBG);

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
  ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)};

static uint16_t buf[ARRAY_SIZE(adc_channels)];
static struct adc_sequence sequence[ARRAY_SIZE(adc_channels)];

// **************************************************************************
// drv_gpio_init
fn_t drv_adc_init(void) {
  int err;
  /* Configure channels individually prior to sampling. */
  for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
    if (!adc_is_ready_dt(&adc_channels[i])) {
      LOG_ERR("ADC controller device %s not ready\n",
              adc_channels[i].dev->name);
      return kFailure;
    }

    err = adc_channel_setup_dt(&adc_channels[i]);
    if (err < 0) {
      LOG_ERR("Could not setup channel #%d (%d)\n", i, err);
      return kFailure;
    }

    sequence[i].buffer = &buf[i];
    sequence[i].buffer_size = sizeof(buf[i]);
  }
  return kSuccess;
}

// **************************************************************************
// drv_adc_update
fn_t drv_adc_update(void) {
  fn_t ret = kSuccess;
  int err;
  for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
    (void)adc_sequence_init_dt(&adc_channels[i], &sequence[i]);
    err = adc_read_dt(&adc_channels[i], &sequence[i]);
    if (err < 0) {
      LOG_ERR("Could not read channel %d: (%d)\n", i, err);
      ret = kFailure;
    }
  }
  return ret;
}

// **************************************************************************
// drv_adc_get
int32_t drv_adc_get(const uint8_t kIdx) {
  int err;
  int32_t val_mv;
  if (ARRAY_SIZE(adc_channels) <= kIdx) {
    LOG_ERR("Invalid channel index: %d\n", kIdx);
    return -1;
  }

  /*
   * If using differential mode, the 16 bit value
   * in the ADC sample buffer should be a signed 2's
   * complement value.
   */
  if (adc_channels[kIdx].channel_cfg.differential) {
    val_mv = (int32_t)((int16_t)buf[kIdx]);
  } else {
    val_mv = (int32_t)buf[kIdx];
  }

  if (0 == kIdx) {
    val_mv *= 5;  // VDDH DIV 5
  }

  err = adc_raw_to_millivolts_dt(&adc_channels[kIdx], &val_mv);
  /* conversion to mV may not be supported, skip if not */
  if (err < 0) {
    LOG_ERR(" (value in mV not available)\n");
  } else {
    return val_mv;
  }
  return -1;
}
