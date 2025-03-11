/**
 * @file hal.c
 * @brief Implementation of hardware abstraction layer for mruby/c on Zephyr
 * @details Provides platform-specific implementations for mruby/c VM operations
 */
#include "hal.h"

#include <stdlib.h>
#include <zephyr/irq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../../../mrubyc/src/mrubyc.h"
#include "../../drv/ble.h"

LOG_MODULE_REGISTER(lib_mrubyc_hal, LOG_LEVEL_DBG);

/** @brief Maximum buffer size for hal_write operations */
#define HAL_WRITE_BUFFER_SIZE 255

#if !defined(MRBC_NO_TIMER)
/* ===== use timer ===== */
/** @brief Storage for IRQ lock key when interrupts are disabled */
static unsigned int hal_irq_lock_key;

/**
 * @brief Enable interrupts
 *
 * @details Unlocks interrupts and the scheduler
 */
void hal_enable_irq(void) {
  irq_unlock(hal_irq_lock_key);
  k_sched_unlock();
}

/**
 * @brief Disable interrupts
 *
 * @details Locks the scheduler and disables interrupts
 */
void hal_disable_irq(void) {
  k_sched_lock();
  hal_irq_lock_key = irq_lock();
}

/**
 * @brief Timer handler for mruby/c VM tick
 *
 * @param timer Timer that triggered the callback
 */
static void mrubyc_haltimerhandler(struct k_timer *const timer) { mrbc_tick(); }

/** @brief Timer definition for mruby/c VM tick */
K_TIMER_DEFINE(mrubyc_haltimer, mrubyc_haltimerhandler, NULL);
#else
/* ===== MRBC_NO_TIMER ===== */
/**
 * @brief Work handler for mruby/c VM tick
 *
 * @param work Work item that triggered the callback
 */
void mrubyc_haltick(struct k_work *const work) { mrbc_tick(); }

/** @brief Work item definition for mruby/c VM tick */
K_WORK_DEFINE(mrubyc_halwork, mrubyc_haltick);

/**
 * @brief Main function for mruby/c VM tick thread
 *
 * @details Continuously submits work items to trigger VM ticks
 *
 * @return int EXIT_FAILURE if the thread exits (should never happen)
 */
static int mrubyc_halmain(void) {
  while (1) {
    k_work_submit(&mrubyc_halwork);
    k_msleep(1);
  }
  return EXIT_FAILURE;
}

/** @brief Thread definition for mruby/c VM tick */
K_THREAD_DEFINE(mrubyc_halthread, 384, mrubyc_halmain, NULL, NULL, NULL, -2,
                K_ESSENTIAL, 0);
#endif

/**
 * @brief Write data to a file descriptor
 *
 * @details Copies data to a buffer and sends it via BLE
 *
 * @param fd File descriptor (ignored)
 * @param buf Buffer containing data to write
 * @param nbytes Number of bytes to write
 * @return int Number of bytes written or negative error code
 */
int hal_write(int fd, const void *buf, int nbytes) {
  char buffer[HAL_WRITE_BUFFER_SIZE] = {0};
  if (HAL_WRITE_BUFFER_SIZE < nbytes) {
    return -1;
  }
  for (int i = 0; i < nbytes; i++) {
    buffer[i] = ((char *)buf)[i];
  }
  ble_print(buffer);
  return nbytes;
}
