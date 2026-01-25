#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#include "function.h"
#include <stdlib.h>

#include "LED.h"
#include "BTN.h"


#define SLEEP_MS 1

#define MAX_FUNCTIONS 10
#define MAX_FUNCTION_TOKENS 128
static Function functions[MAX_FUNCTIONS];
FUNCTION_TOKEN_BUFFER(infix, MAX_FUNCTION_TOKENS);
FUNCTION_TOKEN_BUFFER(postfix, MAX_FUNCTION_TOKENS);

K_HEAP_DEFINE(kheap_postfix, MAX_FUNCTION_TOKENS * 16 * 2);

int main(void) {
  if (BTN_init() < 0) {
    printk("BTN_init: Failed");
    return 0;
  }

  if (LED_init() < 0) {
    printk("LED_init: Failed");
    return 0;
  }

  infix[0] = "3";
  infix[1] = "+";
  infix[2] = "4";
  infix[3] = "*";
  infix[4] = "2";
  infix[5] = "/";
  infix[6] = "(";
  infix[7] = "1";
  infix[8] = "-";
  infix[9] = "5";
  infix[10] = ")";
  infix[11] = "^";
  infix[12] = "2";
  infix[13] = "^";
  infix[14] = "3";
  infix[15] = NULL;

  function_infix_to_postfix(infix, postfix, &kheap_postfix, MAX_FUNCTION_TOKENS);

  char **pfix = postfix;
  while(*pfix) {
    printk("%s ", *pfix);
    pfix++;
  }

  while(1) {
    k_msleep(SLEEP_MS);
  }
}
