#include "fsm.h"

#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/smf.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <stdio.h>

#include "function.h"
#include "graph_display.h"
#include "BTN.h"
#include "keypad.h"
#include "stackf.h"

#define FSM_DEBUG 0

#define MAX_FUNCTIONS 5
#define MAX_FUNCTION_TOKENS 32
static Function functions[MAX_FUNCTIONS];
FUNCTION_TOKEN_BUFFER(infix, MAX_FUNCTION_TOKENS);
FUNCTION_TOKEN_BUFFER(postfix, MAX_FUNCTION_TOKENS);
static char input_buffer[MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH]; 


static void dead_state(void *o);

static void setup_entry(void *o);

static void draw_entry(void *o);
static enum smf_state_result draw_run(void *o);

static void get_function_entry(void *o);
static enum smf_state_result get_function_run(void *o);
static void get_function_exit(void *o);

static void delete_entry(void *o);


enum fsm_state_def {
  DEAD,
  SETUP,
  DRAW,
  GET_FUNCTION,
  DELETE,
};

typedef struct {
  struct smf_ctx ctx;
} fsm_obj_t;


static const struct smf_state states[] = {
  [DEAD] = SMF_CREATE_STATE(dead_state, NULL, NULL, NULL, NULL),
  [SETUP] = SMF_CREATE_STATE(setup_entry, NULL, NULL, NULL, NULL),
  [DRAW] = SMF_CREATE_STATE(draw_entry, draw_run, NULL, NULL, NULL),
  [GET_FUNCTION] = SMF_CREATE_STATE(get_function_entry, get_function_run, get_function_exit, NULL, NULL),
  [DELETE] = SMF_CREATE_STATE(delete_entry, NULL, NULL, NULL, NULL),
};

static fsm_obj_t fsm_obj;

/*
* Primary Functions
*/
int8_t fsm_init(void) { 
  smf_set_initial(SMF_CTX(&fsm_obj), &states[SETUP]);
  return 0;
}

enum smf_state_result fsm_run(void) {
  return smf_run_state(SMF_CTX(&fsm_obj));
}

/*
* State functions
*/
static void dead_state(void *o) {
#if FSM_DEBUG == 1
  printk("State: DEAD\n");
#endif
}

static void setup_entry(void *o) {
#if FSM_DEBUG == 1
  printk("State: SETUP -> entry\n");
  
  // Init on-board button for debugging purposes
  if (BTN_init() < 0) {
    printk("BTN_init(): failed at line %d in %s\n", __LINE__, __FILE__);
  }
#endif
  if (keypad_init() < 0) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[DEAD]);
    return;
  }

  // Initialize display and screen
  if (display_init() < 0) {
    printk("display_init(): failed at line %d in %s\n", __LINE__, __FILE__);
    smf_set_state(SMF_CTX(&fsm_obj), &states[DEAD]);
    return;
  }

  for (uint8_t i = 0; i < MAX_FUNCTIONS; i++) {
    functions[i].is_active = false;
  }
  smf_set_state(SMF_CTX(&fsm_obj), &states[DRAW]);
}

static void draw_entry(void *o) {
#if FSM_DEBUG == 1
  printk("State: DRAW -> entry\n");
#endif
  display_clean();

  graph_draw_axes();
  for (uint8_t i = 0; i < MAX_FUNCTIONS; i++) {
    if (functions[i].is_active) {
      graph_draw_function(&functions[i]);
    }
  }
}

static enum smf_state_result draw_run(void *o) {
#if FSM_DEBUG == 1
  if (BTN_check_clear_pressed(BTN0)) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[GET_FUNCTION]);
  }

  if (BTN_check_clear_pressed(BTN1)) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[DELETE]);
  }
#endif
  //enum key test = get_key();
  //if (test != KEY_NONE) {
  //  printk("%d\n", test);
  //}
  
  enum key key = keypad_get_key();
  if (key == KEY_GET) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[GET_FUNCTION]);
  }
  else if (key == KEY_DEL) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[DELETE]);
  }

  display_timer_handler(); 

  return SMF_EVENT_HANDLED;
}

static void get_function_entry(void *o) {
#if FSM_DEBUG == 1
  printk("State: GET_FUNCTION -> entry\n"); 
#endif
  input_buffer[0] = '\0';

  graph_draw_get_function(input_buffer, GET_FUNCTION_DRAW);
}

static enum smf_state_result get_function_run(void *o) {
#if FSM_DEBUG == 1
  if (BTN_check_clear_pressed(BTN0)) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[DRAW]);
  }

  if (BTN_check_clear_pressed(BTN1)) {
    snprintk(input_buffer, MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH,"%s%s", input_buffer, "sin ( x )");
    graph_draw_get_function(input_buffer, GET_FUNCTION_UPDATE); 
  }

#endif 

  enum key key = keypad_get_key();
  if (key != 255) {
    printk("%d\n", key);
  }

  const char *key_str = keypad_key_to_str(key);

  static bool prev_is_digit = true;
  static size_t cur_len;
  cur_len = 0;
  STACKF_INIT(len_stack);

  // Case for inputting a literal.
  if (key >= KEY_0 && key <= KEY_DOT) { 
    if (!prev_is_digit) {
      snprintk(input_buffer, MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH,"%s%s", input_buffer, " ");
      cur_len++;
    }
    snprintk(input_buffer, MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH,"%s%s", input_buffer, key_str);
    cur_len += strlen(key_str);

    prev_is_digit = true;
    graph_draw_get_function(input_buffer, GET_FUNCTION_UPDATE); 
  }
  // Case for other input keys. 
  else if (key != KEY_NONE && key != KEY_DEL && key != KEY_GET) {
    snprintk(input_buffer, MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH,"%s%s", input_buffer, " ");
    snprintk(input_buffer, MAX_FUNCTION_TOKENS * TOKEN_MAX_LENGTH,"%s%s", input_buffer, key_str);
    cur_len += 1 + strlen(key_str);

    prev_is_digit = false;
    graph_draw_get_function(input_buffer, GET_FUNCTION_UPDATE); 
  } 
  // Case for deleting last input.
  else if (key == KEY_DEL && (int)stackf_peek(&len_stack) != 0) {
    input_buffer[strlen(input_buffer) - (size_t)stackf_pop(&len_stack)] = '\0'; 
    graph_draw_get_function(input_buffer, GET_FUNCTION_UPDATE);
  }
  // Case for exiting and drawing the input function.
  else if (key == KEY_GET) {
    smf_set_state(SMF_CTX(&fsm_obj), &states[DRAW]); 
  }

  if (cur_len > 0) {
    stackf_push(&len_stack, cur_len);
  }

  display_timer_handler();
  return SMF_EVENT_HANDLED;
}

static void get_function_exit(void *o) {
#if FSM_DEBUG == 1
  printk("State: GET_FUNCTION -> exit\n");
#endif
  // Convert the input buffer to the infix token buffer
  uint16_t i = 0;
  char* token = strtok(input_buffer, " ");
  while (token) {
    strcpy(infix[i], token);
    token = strtok(NULL, " ");
    i++;
  }
  infix[i][0] = '\0';

  // Evaluate the infix token buffer as a new function
  for (uint8_t j = 0; j < MAX_FUNCTIONS; j++) {
    if (!functions[j].is_active) {
      if (function_infix_to_postfix(infix, postfix, MAX_FUNCTION_TOKENS) < 0) break;
      float x = X_MIN;
      for (uint16_t k = 0; k < FUNCTION_NUM_POINTS; k++) {
        functions[j].x[k] = x;
        functions[j].y[k] = function_evaluate_postfix(postfix, x);
        x += X_INCREMENT;
      }    
      functions[j].is_active = true;
      break;
    }
  }
}

static void delete_entry(void *o) {
#if FSM_DEBUG == 1
  printk("State: DELETE -> entry\n");
#endif
  uint8_t i = 0;
  while (i < MAX_FUNCTIONS && functions[i].is_active) i++;
  functions[i - 1].is_active = false;
  
  smf_set_state(SMF_CTX(&fsm_obj), &states[DRAW]);
}
