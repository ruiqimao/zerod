#ifndef INIT_SCREEN_H
#define INIT_SCREEN_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace init_screen {

lv_obj_t *init(const printer::State &state);

}
}

#endif