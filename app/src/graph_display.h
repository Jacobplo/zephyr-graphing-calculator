#ifndef GRAPH_DISPLAY_H_
#define GRAPH_DISPLAY_H_

#include "misc/lv_area.h"
#include "stdint.h"
#include "lvgl.h"
#include "function.h"

#include "misc/lv_timer.h"
#include <stdint.h>

#define ABS(a)          ((a) >= 0 ? a : -1 * (a))

#define X_MAX           10
#define X_MIN           -10
#define Y_MAX           10
#define Y_MIN           -10
#define X_MID           ((X_MAX + X_MIN) / 2)
#define Y_MID           ((Y_MAX + Y_MIN) / 2)
#define X_SPAN          (ABS(X_MAX) + ABS(X_MIN))
#define Y_SPAN          (ABS(Y_MAX) + ABS(Y_MIN))
#define X_INCREMENT     ((float)X_SPAN / FUNCTION_NUM_POINTS)

#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           240
#define SCREEN_X_MID            (SCREEN_WIDTH / 2)
#define SCREEN_Y_MID            (SCREEN_HEIGHT / 2)
#define SCREEN_TICK_WIDTH_X     (SCREEN_WIDTH / X_SPAN)
#define SCREEN_TICK_WIDTH_Y     (SCREEN_HEIGHT / Y_SPAN)
#define SCREEN_X_SCALER         ((float)SCREEN_WIDTH / FUNCTION_NUM_POINTS)
#define SCREEN_X_COORD(i)       ((int)((i) * SCREEN_X_SCALER))
#define SCREEN_Y_COORD(y)       ((int)(SCREEN_Y_MID + -1 * (y) * (SCREEN_HEIGHT / Y_SPAN))) 

#define NUM_COLORS      3

enum get_function_state {
  GET_FUNCTION_DRAW,
  GET_FUNCTION_UPDATE
};

int8_t display_init(void);
void display_clean(void);

void graph_draw_axes(void);
void graph_draw_function(Function *func);
void graph_draw_get_function(const char *text, enum get_function_state state);
void graph_draw_line(const lv_point_precise_t *points, size_t num_points, lv_style_t *style);

static inline void display_timer_handler(void) {
  lv_timer_handler();
}

#endif
