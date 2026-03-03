#include "graph_display.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_style.h"

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/printk.h>


#include "function.h"
#include "misc/lv_color.h"
#include "misc/lv_palette.h"
#include "widgets/label/lv_label.h"
#include "widgets/line/lv_line.h"
#include "misc/lv_area.h"
#include "misc/lv_style.h"
#include "misc/lv_style_gen.h"
#include "stdint.h"

#include "lvgl.h"
#include "display/lv_display.h"


static const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static lv_obj_t *screen = NULL;

static const uint32_t color_queue[NUM_COLORS] = { 0xff0000, 0x0000ff, 0x00ff00 };
static uint8_t color_index = 0;

int8_t display_init(void) {
  if(!device_is_ready(display_dev)) {
    printk("device_is_ready(): failed\n");
    return -1;
  }

  screen = lv_screen_active();
  if(screen == NULL) {
    printk("lv_screen_active(): failed\n");
    return -1;
  }

  display_blanking_off(display_dev);

  return 0;
}

void display_clean(void) {
  display_timer_handler();
  lv_obj_clean(screen);
  color_index = 0;
}

void graph_draw_axes(void) {
  static lv_style_t axis_style;
  lv_style_init(&axis_style);
  lv_style_set_line_width(&axis_style, 1);
  lv_style_set_line_color(&axis_style, lv_color_hex(0x000000));
  
  /*
  * Draw axis main lines
  */
  static lv_point_precise_t main_axes[2][2] = {
    { { 0, SCREEN_Y_MID }, { SCREEN_WIDTH, SCREEN_Y_MID } },
    { { SCREEN_X_MID, 0 }, { SCREEN_X_MID, SCREEN_HEIGHT } }
  };
  // x-axis
  graph_draw_line(main_axes[0], 2, &axis_style);
  // y-axis
  graph_draw_line(main_axes[1], 2, &axis_style);

  /*
  * Draw axis ticks
  */
  static lv_point_precise_t axis_xticks[21][2];
  for (int16_t x = 0, i = 0; x < SCREEN_WIDTH; x += SCREEN_TICK_WIDTH_X, i++) {
    axis_xticks[i][0].x = x;
    axis_xticks[i][0].y = SCREEN_Y_MID - 3;
    axis_xticks[i][1].x = x;
    axis_xticks[i][1].y = SCREEN_Y_MID + 4;
    graph_draw_line(axis_xticks[i], 2, &axis_style);
    if (x == SCREEN_X_MID) x--;
  }
  static lv_point_precise_t axis_yticks[21][2];
  for (int16_t y = 0, i = 0; y < SCREEN_HEIGHT; y += SCREEN_TICK_WIDTH_Y, i++) {
    axis_yticks[i][0].x = SCREEN_X_MID - 3;
    axis_yticks[i][0].y = y;
    axis_yticks[i][1].x = SCREEN_X_MID + 4;
    axis_yticks[i][1].y = y;
    graph_draw_line(axis_yticks[i], 2, &axis_style);
    if (y == SCREEN_Y_MID) y--;
  }
}

void graph_draw_function(Function *func) { 
  lv_style_init(&func->style);
  lv_style_set_line_width(&func->style, 2);
  lv_style_set_line_color(&func->style, lv_color_hex(color_queue[color_index]));
  color_index == NUM_COLORS - 1 ? color_index = 0 : color_index++;

  // Default num_asymptotes to 1 because the first the points prior to the first asymptote must be drawn
  uint16_t asymptote_indices[100] = {0}; 
  uint16_t num_asymptotes = 1;
  // Convert all function x and y values to pixel coordinate points, keeping track of asymptotes
  for (uint16_t i = 0; i < FUNCTION_NUM_POINTS; i++) {
    if (i > 0 && (func->y[i] + Y_SPAN < func->y[i - 1] || func->y[i] - Y_SPAN > func->y[i - 1])) {
      asymptote_indices[num_asymptotes] = i;
      num_asymptotes++;
    }

    func->points[i].x = SCREEN_X_COORD(i);
    func->points[i].y = SCREEN_Y_COORD(func->y[i]);
  }
  asymptote_indices[num_asymptotes] = FUNCTION_NUM_POINTS;

  // Draw graph, preventing connection of points on asymptotes
  for (uint16_t i = 0; i < num_asymptotes; i++) {
    graph_draw_line(func->points + asymptote_indices[i], asymptote_indices[i + 1] - asymptote_indices[i], &func->style);
  }
}

void graph_draw_get_function(const char *text, enum get_function_state state) {
  char display_text[TOKEN_MAX_LENGTH * 32] = {0};
  snprintk(display_text, TOKEN_MAX_LENGTH * 32, "y = %s", text);

  static lv_obj_t *box_label;

  if (state == GET_FUNCTION_DRAW) {
    static lv_point_precise_t points[2] = { { 0, 15 }, { SCREEN_WIDTH, 15 } };

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_line_width(&style, 30);
    lv_style_set_line_color(&style, lv_color_hex(0x878787));

    static lv_obj_t *box;
    box = lv_line_create(screen);
    lv_line_set_points(box, points, 2);
    lv_obj_add_style(box, &style, 0);

    box_label = lv_label_create(box);
    lv_label_set_text(box_label, display_text);
    lv_obj_align(box_label, LV_ALIGN_CENTER, 0, 0);
  }

  else if (state == GET_FUNCTION_UPDATE) {
    lv_label_set_text(box_label, display_text);
  }
}

void graph_draw_line(const lv_point_precise_t *points, size_t num_points, lv_style_t *style) {
  lv_obj_t *line = lv_line_create(screen);
  lv_line_set_points(line, points, num_points);
  lv_obj_add_style(line, style, 0);
}

