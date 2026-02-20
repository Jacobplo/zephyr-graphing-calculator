#ifndef FSM_H_
#define FSM_H_

#include <zephyr/smf.h>

int8_t fsm_init(void);

enum smf_state_result fsm_run(void);

#endif
