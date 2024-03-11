#ifndef TEMPERATURE_PAGE_H
#define TEMPERATURE_PAGE_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace temp_page {

lv_obj_t *init(lv_obj_t *parent, const printer::State &state);
void printer_update(const printer::State &state);

}
}

#endif