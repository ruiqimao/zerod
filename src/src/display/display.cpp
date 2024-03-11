#include "display.h"

#include <lvgl.h>
#include <TFT_eSPI.h>

#include "board_conf.h"
#include "cst816s.h"

namespace display {

static TFT_eSPI *_tft = nullptr;
static lv_display_t *_lvd = nullptr;

static lv_color_t _buf1[RES_H * RES_V / 10];
static lv_color_t _buf2[RES_H * RES_V / 10];

static lv_indev_t *_indev = nullptr;

void _flush_display(lv_display_t *display, const lv_area_t *area, uint8_t *color);
void _read_touchscreen(lv_indev_t *indev, lv_indev_data_t *data);

void init() {
  pinMode(LCD_BL_PIN, OUTPUT);

  _tft = new TFT_eSPI();
  _tft->init();
  set_backlight(0);

  _lvd = lv_display_create(RES_H, RES_V);
  lv_display_set_buffers(_lvd, _buf1, _buf2, sizeof(_buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(_lvd, _flush_display);

  cst816s::init(); 

  _indev = lv_indev_create();
  lv_indev_set_type(_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(_indev, _read_touchscreen);

  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_STATE_DEFAULT);
}

void destroy() {
  lv_indev_delete(_indev);
  _indev = nullptr;

  lv_display_delete(_lvd);
  _lvd = nullptr;

  delete _tft;
  _tft = nullptr;
}

void set_backlight(uint8_t target) {
  static uint8_t current = 0;
  if (target > 16) {
    target = 16;
  }

  if (target == current) {
    return;
  }

  if (target == 0) {
    digitalWrite(LCD_BL_PIN, LOW);
    delay(3);
    current = 0;
    return;
  }

  if (current == 0) {
    digitalWrite(LCD_BL_PIN, HIGH);
    delayMicroseconds(25);
    current = 16;
  }

  uint8_t cycles = (current >= target) ? current - target : current + 16 - target;
  for (uint8_t i = 0; i < cycles; i ++) {
    digitalWrite(LCD_BL_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(LCD_BL_PIN, HIGH);
    delayMicroseconds(1);
  }

  current = target;
}

void _flush_display(lv_display_t *display, const lv_area_t *area, uint8_t *color) {
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  _tft->startWrite();
  _tft->setAddrWindow(area->x1, area->y1, w, h);
  _tft->pushColors((uint16_t*) color, w * h, true);
  _tft->endWrite();

  lv_disp_flush_ready(display);
}

void _read_touchscreen(lv_indev_t *indev, lv_indev_data_t *data) {
  if (!cst816s::ready()) {
    return;
  }

  bool pressed = cst816s::read(data->point.x, data->point.y);
  if (pressed) {
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

}