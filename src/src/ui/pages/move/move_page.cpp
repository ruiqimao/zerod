#include "move_page.h"

#include "ui/pages/page_helper.h"
#include "printer/send/send_cmd.h"

namespace ui {
namespace move_page {

lv_obj_t *_xp = nullptr;
lv_obj_t *_yp = nullptr;
lv_obj_t *_zp = nullptr;
lv_obj_t *_xm = nullptr;
lv_obj_t *_ym = nullptr;
lv_obj_t *_zm = nullptr;

void _xp_click_handler(lv_event_t *e);
void _yp_click_handler(lv_event_t *e);
void _zp_click_handler(lv_event_t *e);
void _xm_click_handler(lv_event_t *e);
void _ym_click_handler(lv_event_t *e);
void _zm_click_handler(lv_event_t *e);

lv_obj_t *init(lv_obj_t *parent, const printer::State &state) {
  lv_obj_t *page = page_helper::create_page(parent, "MOVE");

  _xp = page_helper::create_center_button(
      page,
      -55, -38,
      50, 60,
      "X+",
      _xp_click_handler
  );
  _yp = page_helper::create_center_button(
      page,
      0, -38,
      50, 60,
      "Y+",
      _yp_click_handler
  );
  _zp = page_helper::create_center_button(
      page,
      55, -38,
      50, 60,
      "Z+",
      _zp_click_handler
  );
  _xm = page_helper::create_center_button(
      page,
      -55, 28,
      50, 60,
      "X-",
      _xm_click_handler
  );
  _ym = page_helper::create_center_button(
      page,
      0, 28,
      50, 60,
      "Y-",
      _ym_click_handler
  );
  _zm = page_helper::create_center_button(
      page,
      55, 28,
      50, 60,
      "Z-",
      _zm_click_handler
  );

  return page;
}

void printer_update(const printer::State &state) {}

void _xp_click_handler(lv_event_t *e) {
  printer::send::send_move("X+");
}

void _yp_click_handler(lv_event_t *e) {
  printer::send::send_move("Y+");
}

void _zp_click_handler(lv_event_t *e) {
  printer::send::send_move("Z+");
}

void _xm_click_handler(lv_event_t *e) {
  printer::send::send_move("X-");
}

void _ym_click_handler(lv_event_t *e) {
  printer::send::send_move("Y-");
}

void _zm_click_handler(lv_event_t *e) {
  printer::send::send_move("Z-");
}

}
}