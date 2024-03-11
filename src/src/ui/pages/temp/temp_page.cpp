#include "temp_page.h"

#include "ui/pages/page_helper.h"

namespace ui {
namespace temp_page {

static lv_obj_t *_hotend_temp = nullptr;
static lv_obj_t *_hotend_target = nullptr;
static lv_obj_t *_bed_temp = nullptr;
static lv_obj_t *_bed_target = nullptr;
static lv_obj_t *_chamber_temp = nullptr;
static lv_obj_t *_chamber_target = nullptr;

lv_obj_t *_init_temp(lv_obj_t *parent, int y, const char *name);

void _update_temp(lv_obj_t *temp, int value);
void _update_target(lv_obj_t *target, lv_obj_t *temp, int value);

lv_obj_t *init(lv_obj_t *parent, const printer::State &state) {
  lv_obj_t *page = page_helper::create_page(parent, "TEMP");

  _hotend_temp = _init_temp(page, -45, "HOTEND");
  _hotend_target = lv_label_create(page);
  _bed_temp = _init_temp(page, 0, "BED");
  _bed_target = lv_label_create(page);
  _chamber_temp = _init_temp(page, 45, "CHAMBER");
  _chamber_target = lv_label_create(page);

  return page;
}

void printer_update(const printer::State &state) {
  _update_temp(_hotend_temp, state.hotend_temp);
  _update_target(_hotend_target, _hotend_temp, state.hotend_target);
  _update_temp(_bed_temp, state.bed_temp);
  _update_target(_bed_target, _bed_temp, state.bed_target);
  _update_temp(_chamber_temp, state.chamber_temp);
  _update_target(_chamber_target, _chamber_temp, state.chamber_target);
}

lv_obj_t *_init_temp(lv_obj_t *parent, int y, const char *name) {
  lv_obj_t *temp = lv_label_create(parent);
  lv_obj_align(temp, LV_ALIGN_LEFT_MID, 125, y);
  lv_obj_set_style_text_font(temp, &lv_font_montserrat_24, LV_PART_MAIN);

  lv_obj_t *label = lv_label_create(parent);
  lv_obj_align(label, LV_ALIGN_RIGHT_MID, -130, y);
  lv_label_set_text(label, name);

  return temp;
}

void _update_temp(lv_obj_t *temp, int value) {
  lv_label_set_text_fmt(temp, "%d", value);
}

void _update_target(lv_obj_t *target, lv_obj_t *temp, int value) {
  lv_label_set_text_fmt(target, "/ %d", value);
  lv_obj_align_to(target, temp, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, -3);
  if (value == 0) {
    lv_obj_add_flag(target, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(target, LV_OBJ_FLAG_HIDDEN);
  }
}

}
}