#ifndef SCREEN_HELPER_H
#define SCREEN_HELPER_H

#include <lvgl.h>

#include "user_conf.h"

namespace ui {
namespace screen_helper {

inline lv_obj_t *create_screen() {
  lv_obj_t *scr = lv_obj_create(nullptr);
  lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_flag(scr, LV_OBJ_FLAG_SCROLL_ONE);
  lv_obj_set_scroll_dir(scr, LV_DIR_HOR);
  lv_obj_set_scroll_snap_x(scr, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_column(scr, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(scr, COLOR_BG, LV_PART_MAIN);
  return scr;
}

}
}

#endif