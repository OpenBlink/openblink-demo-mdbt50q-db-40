/*! @file
  @brief
  Hardware abstraction layer
        for Zephyr.

  <pre>
  Copyright (C) 2015- Kyushu Institute of Technology.
  Copyright (C) 2015- Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
 */

/**
 * @file hal.h
 * @brief Hardware abstraction layer for mruby/c on Zephyr
 * @details Provides platform-specific implementations for mruby/c VM operations
 */

#ifndef MRBC_SRC_HAL_H_
#define MRBC_SRC_HAL_H_

#include <zephyr/kernel.h>

/** @brief Time unit for mruby/c VM tick in milliseconds */
#define MRBC_TICK_UNIT 1
/** @brief Number of ticks in a timeslice for mruby/c VM scheduling */
#define MRBC_TIMESLICE_TICK_COUNT 10

#if !defined(MRBC_NO_TIMER)

/** @brief Initialize hardware abstraction layer (no-op in this implementation)
 */
#define hal_init() ((void)0)
/**
 * @brief Enable interrupts
 */
void hal_enable_irq(void);
/**
 * @brief Disable interrupts
 */
void hal_disable_irq(void);
/** @brief Idle the CPU for one tick unit */
#define hal_idle_cpu() ((k_msleep(MRBC_TICK_UNIT)))  // delay 1ms

#else

/** @brief Initialize hardware abstraction layer (no-op in this implementation)
 */
#define hal_init() ((void)0)
/** @brief Enable interrupts by unlocking the scheduler */
#define hal_enable_irq() (k_sched_unlock())
/** @brief Disable interrupts by locking the scheduler */
#define hal_disable_irq() (k_sched_lock())
/** @brief Idle the CPU for one tick unit */
#define hal_idle_cpu() ((k_msleep(MRBC_TICK_UNIT)))  // delay 1ms

#endif

/**
 * @brief Write data to a file descriptor
 *
 * @param fd File descriptor
 * @param buf Buffer containing data to write
 * @param nbytes Number of bytes to write
 * @return int Number of bytes written or negative error code
 */
int hal_write(int fd, const void *buf, int nbytes);
/** @brief Flush a file descriptor (no-op in this implementation) */
#define hal_flush(fd) ((void)0)
/** @brief Abort execution with a message (no-op in this implementation) */
#define hal_abort(s) ((void)0)

#endif
