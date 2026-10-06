/**
 * Manages the graphing calculator state machine
 */

#ifndef FSM_H_
#define FSM_H_

#include <zephyr/smf.h>

/**
 * Initializes the state machine
 */
int8_t fsm_init(void);

/**
 * Runs the current state of the state machine
 */
enum smf_state_result fsm_run(void);

#endif
