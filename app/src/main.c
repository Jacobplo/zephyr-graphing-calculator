#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#include "LED.h"
#include "BTN.h"


#define SLEEP_MS 1


int main(void) {
  if (BTN_init() < 0) {
    printk("BTN_init: Failed");
    return 0;
  }

  if (LED_init() < 0) {
    printk("LED_init: Failed");
    return 0;
  }


  while(1) {
    k_msleep(SLEEP_MS);
  }
}
