/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file mrubyc_vm.h
 * @brief mruby/c virtual machine management
 * @details Provides functions for controlling the mruby/c virtual machine
 */
#ifndef APP_MRUBYC_VM_H
#define APP_MRUBYC_VM_H

#include <stdbool.h>

#include "../lib/fn.h"

/**
 * @brief Sets the reload flag for the mruby/c virtual machine
 *
 * @details When set, the VM will reload bytecode on the next cycle
 *
 * @return fn_t kSuccess if successful
 */
fn_t app_mrubyc_vm_set_reload(void);

/**
 * @brief Gets the current state of the reload flag
 *
 * @return true if reload is pending
 * @return false if no reload is pending
 */
bool app_mrubyc_vm_get_reload(void);

#endif
