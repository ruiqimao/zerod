#ifndef IDLE_SCREEN_H
#define IDLE_SCREEN_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace idle_screen {

lv_obj_t *init(const printer::State &state);

}
}

#endif