#ifndef PRINTING_SCREEN_H
#define PRINTING_SCREEN_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace printing_screen {

lv_obj_t *init(const printer::State &state);

}
}

#endif