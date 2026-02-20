#ifndef FSM_H_
#define FSM_H_

#include "zephyr/smf.h"
#include <stdint.h>

void fsm_init(void);

enum smf_state_result fsm_run(void);

#endif
