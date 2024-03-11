#include "print_control_page.h"

#include "ui/pages/page_helper.h"
#include "printer/send/send_cmd.h"
#include "user_conf.h"

namespace ui {
namespace print_control_page {

static bool _paused = false;
static lv_obj_t *_pause = nullptr;
static lv_obj_t *_cancel = nullptr;

void _update_pause();
void _pause_click_handler(lv_event_t *e);
void _cancel_click_handler(lv_event_t *e);

lv_obj_t *init(lv_obj_t *parent, const printer::State &state) {
  lv_obj_t *page = page_helper::create_page(parent, "PRINTING");

  _paused = state.paused;

  _pause = page_helper::create_center_button(
      page,
      0, -38,
      160, 60,
      "",
      _pause_click_handler
  );
  _update_pause();

  _cancel = page_helper::create_center_button(
      page,
      0, 28,
      160, 60,
      "CANCEL",
      _cancel_click_handler
  );
  lv_obj_set_style_bg_color(_cancel, COLOR_CANCEL_BG, LV_PART_MAIN);

  return page;
}

void printer_update(const printer::State &state) {
  if (state.paused != _paused) {
    _paused = state.paused;
    _update_pause();
    lv_obj_clear_state(_pause, LV_STATE_DISABLED);
    lv_obj_clear_state(_cancel, LV_STATE_DISABLED);
  };
}

void _update_pause() {
  lv_obj_t *label = lv_obj_get_child(_pause, 0);
  if (_paused) {
    lv_obj_set_style_bg_color(_pause, COLOR_RESUME_BG, LV_PART_MAIN);
    lv_label_set_text(label, "RESUME");
  } else {
    lv_obj_set_style_bg_color(_pause, COLOR_PAUSE_BG, LV_PART_MAIN);
    lv_label_set_text(label, "PAUSE");
  }
}

void _pause_click_handler(lv_event_t *e) {
  if (_paused) {
    printer::send::send_gcode("RESUME");
  } else {
    printer::send::send_gcode("PAUSE");
  }
  lv_obj_add_state(_pause, LV_STATE_DISABLED);
  lv_obj_add_state(_cancel, LV_STATE_DISABLED);
}

void _cancel_click_handler(lv_event_t *e) {
  printer::send::send_gcode("CANCEL_PRINT");
  lv_obj_add_state(_pause, LV_STATE_DISABLED);
  lv_obj_add_state(_cancel, LV_STATE_DISABLED);
}

}
}