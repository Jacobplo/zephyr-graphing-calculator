#include "graph_display.h"
#include "core/lv_obj_style.h"

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>


#include "widgets/line/lv_line.h"
#include "misc/lv_area.h"
#include "misc/lv_style.h"
#include "misc/lv_style_gen.h"
#include "stdint.h"

#include "lvgl.h"
#include "display/lv_display.h"


static const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static lv_obj_t *screen = NULL;


int8_t display_init(void) {
  if(!device_is_ready(display_dev)) return -1;

  screen = lv_screen_active();
  if(screen == NULL) return -1;

  display_blanking_off(display_dev);

  return 0;
}

void graph_draw_axes(void) {

}

void graph_draw_function(Function *func) {

}

void graph_draw_line(lv_value_precise_t x1, lv_value_precise_t x2, lv_value_precise_t y1, lv_value_precise_t y2) {
  static lv_point_precise_t points[2];
  points[0] = (lv_point_precise_t){ x1, y1 };
  points[1] = (lv_point_precise_t){ x2, y2 };
  
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_line_width(&style, 1);
  lv_style_set_line_color(&style, lv_color_hex(0x0000ff));

  lv_obj_t *line = lv_line_create(screen);
  lv_line_set_points(line, points, 3);
  lv_obj_add_style(line, &style, 0);
  //lv_obj_center(line);
}

