#include "init_screen.h"

#include "ui/ui.h"
#include "user_conf.h"

namespace ui {
namespace init_screen {

static lv_obj_t *name = nullptr;

void _printer_update_handler(const printer::State &state);

lv_obj_t *init(const printer::State &state) {
  lv_obj_t *scr = lv_obj_create(nullptr);
  lv_obj_set_style_bg_color(scr, COLOR_BG, LV_PART_MAIN);
  control::register_printer_update_cb(scr, _printer_update_handler);

  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, "WAITING");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);

  lv_obj_t *spinner = lv_spinner_create(scr);
  lv_obj_set_size(spinner, 60, 60);
  lv_obj_center(spinner);
  lv_obj_set_style_arc_width(spinner, 5, LV_PART_MAIN);
  lv_obj_set_style_arc_width(spinner, 5, LV_PART_INDICATOR);

  name = lv_label_create(scr);
  lv_label_set_text(name, PRINTER_NAME);
  lv_obj_set_style_text_align(name, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
  lv_obj_align(name, LV_ALIGN_BOTTOM_MID, 0, -15);

  return scr;
}

void _printer_update_handler(const printer::State &state) {
  if (state.gcodes[0] != '\0') {
    lv_label_set_text(name, state.gcodes);
  }
}

}
}