#include "shutdown_screen.h"

#include "printer/send/send_cmd.h"
#include "ui/pages/page_helper.h"
#include "ui/ui.h"

namespace ui {
namespace shutdown_screen {

static lv_obj_t *_restart = nullptr;
static lv_obj_t *_message = nullptr;

void _printer_update_handler(const printer::State &state);
void _restart_click_handler(lv_event_t *e);

lv_obj_t *init(const printer::State &state) {
  lv_obj_t *scr = lv_obj_create(nullptr);
  lv_obj_set_style_bg_color(scr, COLOR_BG, LV_PART_MAIN);
  control::register_printer_update_cb(scr, _printer_update_handler);

  lv_obj_t *title = lv_label_create(scr);
  lv_label_set_text(title, "SHUTDOWN");
  lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 15);

  _message = lv_label_create(scr);
  lv_label_set_text(_message, "");
  lv_obj_set_width(_message, 180);
  lv_obj_align(_message, LV_ALIGN_CENTER, 0, -25);
  lv_obj_set_style_text_align(_message, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);

  _restart = page_helper::create_center_button(
    scr,
    0, 60,
    150, 50,
    "RESTART",
    _restart_click_handler
  );

  return scr;
}

void _printer_update_handler(const printer::State &state) {
  if (state.gcodes[0] != '\0') {
    lv_label_set_text(_message, state.gcodes);
  }
}

void _restart_click_handler(lv_event_t *e) {
  printer::send::send_cmd(printer::send::Commands::kRestart);
  lv_obj_add_state(_restart, LV_STATE_DISABLED);
}

}
}