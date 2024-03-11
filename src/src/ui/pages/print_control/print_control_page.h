#ifndef PRINT_CONTROL_PAGE_H
#define PRINT_CONTROL_PAGE_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace print_control_page {

lv_obj_t *init(lv_obj_t *parent, const printer::State &state);
void printer_update(const printer::State &state);

}
}

#endif