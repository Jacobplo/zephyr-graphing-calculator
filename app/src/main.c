#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#include "fsm.h"


#define SLEEP_MS 1

int main(void) { 
  if (fsm_init() < 0) {
    printk("fsm_init(): failed\n");
  }

  while(1) {
    fsm_run();
    k_msleep(SLEEP_MS);
  }
}
