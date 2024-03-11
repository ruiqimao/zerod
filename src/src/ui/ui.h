#ifndef UI_H
#define UI_H

#include <functional>
#include <lvgl.h>

#include "printer/printer.h"

namespace ui {

typedef std::function<lv_obj_t*(const printer::State&)> scr_init_t;
typedef void (*printer_update_cb_t)(const printer::State&);

void init();
void update(const printer::State &state);

namespace control {

void register_printer_update_cb(lv_obj_t *obj, printer_update_cb_t cb);

void scroll_right();
void scroll_left();

}

}

#endif