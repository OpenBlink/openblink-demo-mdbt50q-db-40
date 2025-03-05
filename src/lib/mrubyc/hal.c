#include "hal.h"

#include <stdlib.h>
#include <zephyr/irq.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../../../mrubyc/src/mrubyc.h"
#include "../../drv/ble.h"

LOG_MODULE_REGISTER(lib_mrubyc_hal, LOG_LEVEL_DBG);

#define HAL_WRITE_BUFFER_SIZE 255

#if !defined(MRBC_NO_TIMER)
/* ===== use timer ===== */
static unsigned int hal_irq_lock_key;
void hal_enable_irq(void) {
  irq_unlock(hal_irq_lock_key);
  k_sched_unlock();
}
void hal_disable_irq(void) {
  k_sched_lock();
  hal_irq_lock_key = irq_lock();
}
static void mrubyc_haltimerhandler(struct k_timer *const timer) { mrbc_tick(); }
K_TIMER_DEFINE(mrubyc_haltimer, mrubyc_haltimerhandler, NULL);
#else
/* ===== MRBC_NO_TIMER ===== */
void mrubyc_haltick(struct k_work *const work) { mrbc_tick(); }
K_WORK_DEFINE(mrubyc_halwork, mrubyc_haltick);
static int mrubyc_halmain(void) {
  while (1) {
    k_work_submit(&mrubyc_halwork);
    k_msleep(1);
  }
  return EXIT_FAILURE;
}
K_THREAD_DEFINE(mrubyc_halthread, 384, mrubyc_halmain, NULL, NULL, NULL, -2,
                K_ESSENTIAL, 0);
#endif

/* ===== hal_write ===== */
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
