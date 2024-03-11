#include "printing_page.h"

#include <lvgl.h>

#include "ui/pages/page_helper.h"

namespace ui {
namespace printing_page {

static lv_obj_t *_progress = nullptr;
static lv_obj_t *_hotend = nullptr;
static lv_obj_t *_bed = nullptr;
static lv_obj_t *_chamber = nullptr;

lv_obj_t *init(lv_obj_t *parent, const printer::State &state) {
  lv_obj_t *page = page_helper::create_page(parent, "PRINTING");

  _progress = lv_label_create(page);
  lv_obj_align(_progress, LV_ALIGN_CENTER, 0, -30);
  lv_obj_set_style_text_font(_progress, &lv_font_montserrat_36, LV_PART_MAIN);

  lv_obj_t *hotend_label = lv_label_create(page);
  lv_obj_align(hotend_label, LV_ALIGN_CENTER, -50, 40);
  lv_label_set_text(hotend_label, "H");

  lv_obj_t *bed_label = lv_label_create(page);
  lv_obj_align(bed_label, LV_ALIGN_CENTER, 0, 40);
  lv_label_set_text(bed_label, "B");

  lv_obj_t *chamber_label = lv_label_create(page);
  lv_obj_align(chamber_label, LV_ALIGN_CENTER, 50, 40);
  lv_label_set_text(chamber_label, "C");

  _hotend = lv_label_create(page);
  lv_obj_align(_hotend, LV_ALIGN_CENTER, -50, 60);

  _bed = lv_label_create(page);
  lv_obj_align(_bed, LV_ALIGN_CENTER, 0, 60);

  _chamber = lv_label_create(page);
  lv_obj_align(_chamber, LV_ALIGN_CENTER, 50, 60);

  return page;
}

void printer_update(const printer::State &state) {
  lv_label_set_text_fmt(_progress, "%d%%", state.progress);
  lv_label_set_text_fmt(_hotend, "%d", state.hotend_temp);
  lv_label_set_text_fmt(_bed, "%d", state.bed_temp);
  lv_label_set_text_fmt(_chamber, "%d", state.chamber_temp);
}

}
}