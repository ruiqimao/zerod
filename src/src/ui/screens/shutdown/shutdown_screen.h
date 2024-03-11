#ifndef SHUTDOWN_SCREEN_H
#define SHUTDOWN_SCREEN_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace shutdown_screen {

lv_obj_t *init(const printer::State &state);

}
}

#endif