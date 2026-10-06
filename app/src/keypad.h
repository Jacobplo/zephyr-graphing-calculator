#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


#define NUM_ROWS 4
#define NUM_COLS 7
#define ZEPHYR_USER_NODE DT_PATH(zephyr_user)
#define GPIO(id, idx) GPIO_DT_SPEC_GET_BY_IDX(ZEPHYR_USER_NODE, id, idx)

enum key {
  KEY_0, KEY_1, KEY_2,
  KEY_3, KEY_4, KEY_5,
  KEY_6, KEY_7, KEY_8,
  KEY_9, KEY_NEG, KEY_DOT,
  KEY_SIN, KEY_COS, KEY_TAN,
  KEY_RB, KEY_LB, KEY_ADD,
  KEY_MIN, KEY_MUL, KEY_DIV,
  KEY_X, KEY_PI, KEY_E,
  KEY_POW, KEY_LN, KEY_SQRT,
  KEY_ABS, KEY_DEL, KEY_GET,
  KEY_NONE
};

int keypad_init(void);

enum key keypad_get_key(void);

const char *keypad_key_to_str(enum key key);

#endif
