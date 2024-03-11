#include "gcode_page.h"

#include "ui/pages/page_helper.h"
#include "printer/send/send_cmd.h"

namespace ui {
namespace gcode_page {

static lv_obj_t *_roller = nullptr;
static lv_obj_t *_run = nullptr;

void _run_click_handler(lv_event_t *e);

lv_obj_t *init(lv_obj_t *parent, const printer::State &state) {
  lv_obj_t *page = page_helper::create_page(parent, "GCODE");

  _roller = lv_roller_create(page);
  lv_roller_set_options(_roller, state.gcodes, LV_ROLLER_MODE_INFINITE);
  lv_obj_set_size(_roller, 160, 110);
  lv_obj_align(_roller, LV_ALIGN_CENTER, 0, -25);
  lv_obj_set_style_text_color(_roller, COLOR_GCODE_UNSELECTED, LV_PART_MAIN);
  lv_obj_set_style_text_align(_roller, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
  lv_obj_set_style_text_font(_roller, &lv_font_montserrat_16, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(_roller, 0, LV_PART_MAIN);
  lv_obj_set_style_border_width(_roller, 0, LV_PART_MAIN);

  lv_obj_set_style_text_align(_roller, LV_TEXT_ALIGN_LEFT, LV_PART_SELECTED);
  lv_obj_set_style_radius(_roller, 10, LV_PART_SELECTED);
  lv_obj_set_style_bg_color(_roller, COLOR_GCODE_HIGHLIGHT, LV_PART_SELECTED);

  _run = page_helper::create_center_button(
      page,
      0, 55,
      160, 40,
      "RUN",
      _run_click_handler
  );

  return page;
}

void printer_update(const printer::State &state) {
  lv_obj_set_state(_run, LV_STATE_DISABLED, state.working);
}

void _run_click_handler(lv_event_t *e) {
  static char selected[sizeof(printer::State::gcodes)];
  lv_roller_get_selected_str(_roller, selected, sizeof(selected));
  printer::send::send_gcode(selected);
}

}
}