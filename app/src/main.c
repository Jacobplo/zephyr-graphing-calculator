#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#include "function.h"

#include "LED.h"
#include "BTN.h"


#define SLEEP_MS 1

#define MAX_FUNCTIONS 10
#define MAX_FUNCTION_TOKENS 128
static Function functions[MAX_FUNCTIONS];
FUNCTION_TOKEN_BUFFER(infix, MAX_FUNCTION_TOKENS);
FUNCTION_TOKEN_BUFFER(postfix, MAX_FUNCTION_TOKENS);

int main(void) {
  if (BTN_init() < 0) {
    printk("BTN_init: Failed");
    return 0;
  }

  if (LED_init() < 0) {
    printk("LED_init: Failed");
    return 0;
  }

  strcpy(infix[0], "3");
  strcpy(infix[1], "+");
  strcpy(infix[2], "4");
  strcpy(infix[3], "*");
  strcpy(infix[4], "2");
  strcpy(infix[5], "/");
  strcpy(infix[6], "(");
  strcpy(infix[7], "1");
  strcpy(infix[8], "-");
  strcpy(infix[9], "5");
  strcpy(infix[10], ")");
  strcpy(infix[11], "^");
  strcpy(infix[12], "2");
  strcpy(infix[13], "^");
  strcpy(infix[14], "3");
  infix[15][0] = '\0';

  function_infix_to_postfix(infix, postfix, MAX_FUNCTION_TOKENS);

  for(int i = 0; i < MAX_FUNCTION_TOKENS && postfix[i][0] != '\0'; i++) {
    printk("%s ", postfix[i]);  
  }

  while(1) {
    k_msleep(SLEEP_MS);
  }
}
