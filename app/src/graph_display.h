#ifndef GRAPH_DISPLAY_H_
#define GRAPH_DISPLAY_H_

#include "misc/lv_area.h"
#include "stdint.h"
#include "lvgl.h"
#include "function.h"

#include "misc/lv_timer.h"
#include <stdint.h>


#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define SCREEN_X_MID        (SCREEN_WIDTH / 2)
#define SCREEN_Y_MID        (SCREEN_HEIGHT / 2)
#define SCREEN_TICK_WIDTH_X (SCREEN_WIDTH / 20)
#define SCREEN_TICK_WIDTH_Y (SCREEN_HEIGHT / 20)

#define X_MAX           10
#define X_MIN           -10
#define Y_MAX           10
#define Y_MIN           -10
#define X_MID           ((X_MAX + X_MIN) / 2)
#define Y_MID           ((Y_MAX + Y_MIN) / 2)


int8_t display_init(void);
void graph_draw_axes(void);
void graph_draw_function(Function *func);
void graph_draw_line(const lv_point_precise_t *points, size_t num_points, uint32_t color_hex);

static inline void display_timer_handler(void) {
  lv_timer_handler();
}

#endif
