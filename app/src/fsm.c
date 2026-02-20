#include "fsm.h"

#include <zephyr/smf.h>
#include <zephyr/sys/printk.h>

#include "function.h"
#include "graph_display.h"

#define FSM_DEBUG 0

#define MAX_FUNCTIONS 5
#define MAX_FUNCTION_TOKENS 32
static Function functions[MAX_FUNCTIONS];
FUNCTION_TOKEN_BUFFER(infix, MAX_FUNCTION_TOKENS);
FUNCTION_TOKEN_BUFFER(postfix, MAX_FUNCTION_TOKENS);

static void setup_entry(void *o);
static enum smf_state_result setup_run(void *o);
static void setup_exit(void *o);


enum fsm_state_def {
  SETUP,
};

typedef struct {
  struct smf_ctx ctx;
} fsm_obj_t;


static const struct smf_state states[] = {
  [SETUP] = SMF_CREATE_STATE(setup_entry, setup_run, setup_exit, NULL, NULL),
};

static fsm_obj_t fsm_obj;

/*
* Primary Functions
*/
int8_t fsm_init(void) {
  if (display_init() < 0) {
    printk("display_init(): failed\n");
    return -1;
  }

  // FIXME TEMPORARY
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
   
  graph_draw_axes();
  graph_draw_function(&functions[0]);
  graph_draw_function(&functions[1]);
  // END

  smf_set_initial(SMF_CTX(&fsm_obj), &states[SETUP]);
  return 0;
}

enum smf_state_result fsm_run(void) {
  // FIXME TEMPORARY
  display_timer_handler();
  // END
  
  return smf_run_state(SMF_CTX(&fsm_obj));
}

/*
* State functions
*/
static void setup_entry(void *o) {
  return;
}

static enum smf_state_result setup_run(void *o) {
  return SMF_EVENT_HANDLED;
}

static void setup_exit(void *o) {
  return;
}

