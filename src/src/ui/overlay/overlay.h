#ifndef OVERLAY_H
#define OVERLAY_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace overlay {

lv_obj_t *init(const printer::State &state);

}
}

#endif