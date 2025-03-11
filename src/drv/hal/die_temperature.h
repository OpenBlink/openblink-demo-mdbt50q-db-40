/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file die_temperature.h
 * @brief Die temperature sensor interface
 * @details Provides functions for reading the die temperature
 */
#ifndef HAL_DIE_TEMPERATURE_H
#define HAL_DIE_TEMPERATURE_H

#include "../../lib/fn.h"

/**
 * @brief Gets the current die temperature
 *
 * @return float Temperature in degrees Celsius
 */
float hal_die_temperature_get(void);

#endif
