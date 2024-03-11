#ifndef PAGE_HELPER_H
#define PAGE_HELPER_H

#include <lvgl.h>

#include "board_conf.h"
#include "user_conf.h"

namespace ui {
namespace page_helper {

inline lv_obj_t *create_page(lv_obj_t *parent, const char *title) {
  lv_obj_t *page = lv_obj_create(parent);
  lv_obj_remove_style_all(page);
  lv_obj_set_size(page, RES_H, RES_V);

  lv_obj_t *title_obj = lv_label_create(page);
  lv_label_set_text(title_obj, title);
  lv_obj_align(title_obj, LV_ALIGN_TOP_MID, 0, 15);

  return page;
}

inline lv_obj_t *create_center_button(
    lv_obj_t *parent,
    int x, int y,
    int w, int h,
    const char *label_text,
    lv_event_cb_t cb) {
  lv_obj_t *btn = lv_button_create(parent);
  lv_obj_set_size(btn, w, h);
  lv_obj_align(btn, LV_ALIGN_CENTER, x, y);
  lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_set_style_bg_color(btn, COLOR_BTN_BG, LV_PART_MAIN);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, label_text);
  lv_obj_center(label);

  return btn;
}

}
}

#endif