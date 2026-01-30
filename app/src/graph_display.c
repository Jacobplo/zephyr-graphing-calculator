#include "graph_display.h"

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>

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
