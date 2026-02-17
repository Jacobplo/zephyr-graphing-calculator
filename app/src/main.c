#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>

#include "function.h"
#include "graph_display.h"

#include "LED.h"
#include "BTN.h"


#define SLEEP_MS 1

#define MAX_FUNCTIONS 5
#define MAX_FUNCTION_TOKENS 32
static Function functions[MAX_FUNCTIONS];
FUNCTION_TOKEN_BUFFER(infix, MAX_FUNCTION_TOKENS);
FUNCTION_TOKEN_BUFFER(postfix, MAX_FUNCTION_TOKENS);

int main(void) {
  if (display_init() < 0) {
    printk("display_init: failed\n");
    return 0;
  }

  if (BTN_init() < 0) {
    printk("BTN_init: Failed");
    return 0;
  }

  if (LED_init() < 0) {
    printk("LED_init: Failed");
    return 0;
  }

  //strcpy(infix[0], "3");
  //strcpy(infix[1], "+");
  //strcpy(infix[2], "4");
  //strcpy(infix[3], "*");
  //strcpy(infix[4], "2");
  //strcpy(infix[5], "/");
  //strcpy(infix[6], "(");
  //strcpy(infix[7], "1");
  //strcpy(infix[8], "-");
  //strcpy(infix[9], "5");
  //strcpy(infix[10], ")");
  //strcpy(infix[11], "^");
  //strcpy(infix[12], "2");
  //strcpy(infix[13], "*");
  //strcpy(infix[14], "sin");
  //strcpy(infix[15], "(");
  //strcpy(infix[16], "x");
  //strcpy(infix[17], ")");
  //infix[18][0] = '\0';
 
  strcpy(infix[0], "tan");
  strcpy(infix[1], "(");
  strcpy(infix[2], "x");
  strcpy(infix[3], ")");
  infix[4][0] = '\0';


  function_infix_to_postfix(infix, postfix, MAX_FUNCTION_TOKENS);

  for(int i = 0; i < MAX_FUNCTION_TOKENS && postfix[i][0] != '\0'; i++) {
    printk("%s ", postfix[i]);  
  }
  printk("\n");

  float x = X_MIN;
  for (int16_t i = 0; i < FUNCTION_NUM_POINTS; i++) {
    functions[0].x[i] = x;
    functions[0].y[i] = function_evaluate_postfix(postfix, x);
    x += X_INCREMENT;
  }

  strcpy(infix[0], "x");
  strcpy(infix[1], "^");
  strcpy(infix[2], "2");
  infix[3][0] = '\0';

  function_infix_to_postfix(infix, postfix, MAX_FUNCTION_TOKENS);

  for(int i = 0; i < MAX_FUNCTION_TOKENS && postfix[i][0] != '\0'; i++) {
    printk("%s ", postfix[i]);  
  }
  printk("\n");

  x = X_MIN;
  for (int16_t i = 0; i < FUNCTION_NUM_POINTS; i++) {
    functions[1].x[i] = x;
    functions[1].y[i] = function_evaluate_postfix(postfix, x);
    x += X_INCREMENT;
  }
    

  //graph_draw_line(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0xff0000);
  graph_draw_axes();
  graph_draw_function(&functions[0]);
  graph_draw_function(&functions[1]);


  while(1) {
    display_timer_handler();
    k_msleep(SLEEP_MS);
  }
}
