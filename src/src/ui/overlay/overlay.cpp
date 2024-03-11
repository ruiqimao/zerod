#include "overlay.h"

#include "board_conf.h"
#include "printer/send/send_cmd.h"
#include "ui/ui.h"
#include "user_conf.h"

namespace ui {
namespace overlay {

static lv_obj_t *_progress = nullptr;

lv_obj_t *_init_arc(int start, int end);
void _arc_hit_handler(lv_event_t *e);

void _printer_update_handler(const printer::State &state);

void _stop_click_handler(lv_event_t *e);
void _right_click_handler(lv_event_t *e);
void _left_click_handler(lv_event_t *e);

lv_obj_t *init(const printer::State &state) {
  lv_obj_t *scr = lv_obj_create(nullptr);
  control::register_printer_update_cb(scr, _printer_update_handler);

  _progress = lv_arc_create(lv_layer_top());
  lv_obj_set_size(_progress, 240, 240);
  lv_obj_center(_progress);
  lv_arc_set_bg_angles(_progress, 120, 60);
  lv_arc_set_range(_progress, 0, 100);
  lv_obj_set_style_arc_width(_progress, 3, LV_PART_MAIN);
  lv_obj_set_style_arc_width(_progress, 3, LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(_progress, COLOR_PROGRESS_ARC, LV_PART_INDICATOR);
  lv_obj_set_style_opa(_progress, 0, LV_PART_KNOB);
  lv_obj_remove_flag(_progress, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(_progress, LV_OBJ_FLAG_HIDDEN);

  lv_obj_t *stop = lv_button_create(lv_layer_top());
  lv_obj_set_size(stop, RES_H, 35);
  lv_obj_align(stop, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(stop, COLOR_STOP_BG, LV_PART_MAIN);

  lv_obj_t *stop_label = lv_label_create(stop);
  lv_label_set_text(stop_label, "STOP");
  lv_obj_center(stop_label);

  lv_obj_add_event_cb(
      stop,
      _stop_click_handler,
      LV_EVENT_CLICKED,
      nullptr
  );

  lv_obj_t *right = _init_arc(350, 10);
  lv_obj_add_event_cb(
      right,
      _right_click_handler,
      LV_EVENT_CLICKED,
      nullptr
  );

  lv_obj_t *left = _init_arc(170, 190);
  lv_obj_add_event_cb(
      left,
      _left_click_handler,
      LV_EVENT_CLICKED,
      nullptr
  );

  return scr;
}

lv_obj_t *_init_arc(int start, int end) {
  lv_obj_t *obj = lv_arc_create(lv_layer_top());
  lv_obj_set_width(obj, RES_H - 20);
  lv_obj_set_height(obj, RES_V - 20);
  lv_obj_set_align(obj, LV_ALIGN_CENTER);
  lv_arc_set_bg_angles(obj, start, end);
  lv_obj_set_style_arc_color(obj, COLOR_SIDE_ARC, LV_PART_MAIN);
  lv_obj_set_style_arc_width(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(obj, 8, LV_PART_MAIN | LV_STATE_PRESSED);
#if SHOW_SIDE_ARCS == 0
  lv_obj_set_style_opa(obj, 0, LV_PART_MAIN);
#endif
  lv_obj_set_style_opa(obj, 0, LV_PART_INDICATOR);
  lv_obj_set_style_opa(obj, 0, LV_PART_KNOB);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_add_event_cb(
      obj,
      _arc_hit_handler,
      LV_EVENT_HIT_TEST,
      nullptr
  );
  return obj;
}

void _arc_hit_handler(lv_event_t *e) {
  lv_obj_t *obj = (lv_obj_t*) lv_event_get_current_target(e);
  lv_hit_test_info_t *info = (lv_hit_test_info_t*) lv_event_get_param(e);
  if (lv_arc_get_bg_angle_end(obj) < 180) {
    info->res = info->point->x > RES_H - 30;
  } else {
    info->res = info->point->x < 30;
  }
}

void _printer_update_handler(const printer::State &state) {
  if (state.status == printer::Status::kDisconnected ||
      state.status == printer::Status::kShutdown) {
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_HIDDEN);
  }

  if (state.status == printer::Status::kPrinting) {
    lv_arc_set_value(_progress, state.progress);
    lv_obj_clear_flag(_progress, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_flag(_progress, LV_OBJ_FLAG_HIDDEN);
  }
}

void _stop_click_handler(lv_event_t *e) {
  printer::send::send_stop();
}

void _right_click_handler(lv_event_t *e) {
  control::scroll_right();
}

void _left_click_handler(lv_event_t *e) {
  control::scroll_left();
}

}
}