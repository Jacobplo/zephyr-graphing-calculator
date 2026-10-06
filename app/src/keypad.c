#include "keypad.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec row[NUM_ROWS] = {
    GPIO(row_gpios, 0), GPIO(row_gpios, 1), GPIO(row_gpios, 2),
    GPIO(row_gpios, 3)};
static const struct gpio_dt_spec col[NUM_COLS] = {
    GPIO(col_gpios, 0), GPIO(col_gpios, 1), GPIO(col_gpios, 2),
    GPIO(col_gpios, 3), GPIO(col_gpios, 4), GPIO(col_gpios, 5),
    GPIO(col_gpios, 6)};

static const char *key_map[] = {
  [KEY_0] = "0",
  [KEY_1] = "1",
  [KEY_2] = "2",
  [KEY_3] = "3",
  [KEY_4] = "4",
  [KEY_5] = "5",
  [KEY_6] = "6",
  [KEY_7] = "7",
  [KEY_8] = "8",
  [KEY_9] = "9",
  [KEY_NEG] = "-",
  [KEY_DOT] = ".",
  [KEY_SIN] = "sin",
  [KEY_COS] = "cos",
  [KEY_TAN] = "tan",
  [KEY_RB] = ")",
  [KEY_LB] = "(",
  [KEY_ADD] = "+",
  [KEY_MIN] = "-",
  [KEY_MUL] = "*",
  [KEY_DIV] = "/",
  [KEY_X] = "x",
  [KEY_PI] = "pi",
  [KEY_E] = "e",
  [KEY_POW] = "^",
  [KEY_LN] = "ln",
  [KEY_SQRT] = "sqrt",
  [KEY_ABS] = "abs",
};

static uint8_t keys[NUM_ROWS][NUM_COLS] = {
  { KEY_7, KEY_8,   KEY_9,   KEY_DIV, KEY_POW, KEY_SIN, KEY_GET  },
  { KEY_4, KEY_5,   KEY_6,   KEY_MUL, KEY_LB , KEY_COS, KEY_DEL  },
  { KEY_1, KEY_2,   KEY_3,   KEY_MIN, KEY_RB , KEY_TAN, KEY_SQRT },
  { KEY_0, KEY_DOT, KEY_NEG, KEY_ADD, KEY_X  , KEY_LN , KEY_E    },
};

static void keypad_select_row(uint8_t row_num);

int keypad_init() {
  // Initialize row GPIO pins
  for (uint8_t i = 0; i < NUM_ROWS; i++) {
    if (!gpio_is_ready_dt(&row[i])) {
      printk("gpio_is_read_dt(): failed at line %d in %s\n", __LINE__, __FILE__);
      return -1;
    }

    if (gpio_pin_configure_dt(&row[i], GPIO_OUTPUT_HIGH)) {
      printk("gpio_pin_configure_dt(): failed at line %d in %s\n", __LINE__, __FILE__);
      return -1;
    }
  }

  // Initialize column GPIO pins
  for (uint8_t i = 0; i < NUM_COLS; i++) {
    if (!gpio_is_ready_dt(&col[i])) {
      printk("gpio_is_read_dt(): failed at line %d in %s\n", __LINE__, __FILE__);
      return -1;
    }
    if (gpio_pin_configure_dt(&col[i], GPIO_INPUT)) {
      printk("gpio_pin_configure_dt(): failed at line %d in %s\n", __LINE__, __FILE__);
      return -1;
    } 
  }

  return 0;
}

enum key keypad_get_key() {
  static bool lifted = true;
  static bool pressed = false;

  k_msleep(10);
  for(int8_t i = 0; i < NUM_ROWS; i++) {
    keypad_select_row(i);
    k_msleep(10);
    for(int8_t j = 0; j < NUM_COLS; j++) {
      if(gpio_pin_get_dt(&col[j])) {
        pressed = true;
        if (lifted) { 
          lifted = false;
          return keys[i][j];
        }
      } 
    }
  }
  if (!pressed) {
    lifted = true;
  }
  pressed = false;

  return KEY_NONE;
}

const char *keypad_key_to_str(enum key key) {
  return key_map[key];
}

static void keypad_select_row(uint8_t row_num) {
  // Reset previous row to floating
  gpio_pin_configure_dt(&row[(row_num - 1 + NUM_ROWS) % NUM_ROWS], GPIO_OUTPUT_HIGH);

  // Set selected row to HIGH
  gpio_pin_configure_dt(&row[row_num], GPIO_OUTPUT_LOW);
}
