#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <lvgl.h>

#include "printer/printer.h"

namespace ui {
namespace home_page {

lv_obj_t *init(lv_obj_t *parent, const printer::State &state);
void printer_update(const printer::State &state);

}
}

#endif