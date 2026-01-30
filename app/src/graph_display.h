#ifndef GRAPH_DISPLAY_H_
#define GRAPH_DISPLAY_H_

#include "stdint.h"
#include "lvgl.h"
#include "function.h"

#include "misc/lv_timer.h"


#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240


int8_t display_init(void);
void graph_draw_axes(void);
void graph_draw_function(Function *func);

static inline void display_timer_handler(void) {
  lv_timer_handler();
}

#endif
