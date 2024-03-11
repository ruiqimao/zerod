#include "ui.h"

#include <Arduino.h>

#include "board_conf.h"
#include "printer/printer.h"
#include "ui/overlay/overlay.h"
#include "ui/screens/idle/idle_screen.h"
#include "ui/screens/init/init_screen.h"
#include "ui/screens/printing/printing_screen.h"
#include "ui/screens/shutdown/shutdown_screen.h"
#include "user_conf.h"

namespace ui {

static lv_event_code_t _lv_event_printer_update;

static printer::Status _status = printer::Status::kDisconnected;
static lv_obj_t *_scr = nullptr;
static lv_obj_t *_overlay = nullptr;

void init() {
  lv_init();
  lv_tick_set_cb([]() { return (uint32_t) millis(); });
  _lv_event_printer_update = static_cast<lv_event_code_t>(lv_event_register_id());
}

void update(const printer::State &state) {
  if (!_overlay) {
    _overlay = overlay::init(state);
  }

  scr_init_t next_scr_init = nullptr;
  if (state.status != _status || !_scr) {
    switch (state.status) {
    case printer::Status::kDisconnected:
      next_scr_init = init_screen::init;
      break;
    case printer::Status::kIdle:
      next_scr_init = idle_screen::init;
      break;
    case printer::Status::kPrinting:
      next_scr_init = printing_screen::init;
      break;
    case printer::Status::kShutdown:
      next_scr_init = shutdown_screen::init;
      break;
    }
    _status = state.status;
  }

  if (next_scr_init) {
    _scr = next_scr_init(state);
    lv_screen_load_anim(_scr, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, true);
  }

  if (_scr) {
    lv_obj_send_event(_scr, _lv_event_printer_update, (void*) &state);
  }
  lv_obj_send_event(_overlay, _lv_event_printer_update, (void*) &state);
}

namespace control {

void _printer_update_cb(lv_event_t *e);

void register_printer_update_cb(lv_obj_t *obj, printer_update_cb_t cb) {
  lv_obj_add_event_cb(
      obj,
      _printer_update_cb,
      _lv_event_printer_update,
      (void*) cb
  );
}

void scroll_right() {
  if (_scr) {
    int32_t scroll_by = -lv_obj_get_scroll_right(_scr);
    if (scroll_by < -RES_H) {
      scroll_by = -RES_H;
    }
    lv_obj_scroll_by(_scr, scroll_by, 0, LV_ANIM_ON);
  }
}

void scroll_left() {
  if (_scr) {
    int32_t scroll_by = lv_obj_get_scroll_left(_scr);
    if (scroll_by > RES_H) {
      scroll_by = RES_H;
    }
    lv_obj_scroll_by(_scr, scroll_by, 0, LV_ANIM_ON);
  }
}

void _printer_update_cb(lv_event_t *e) {
  printer::State *state = (printer::State*) lv_event_get_param(e);
  printer_update_cb_t cb = (printer_update_cb_t) lv_event_get_user_data(e);
  cb(*state);
}

}

}