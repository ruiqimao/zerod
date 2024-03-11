#include "printing_screen.h"

#include "ui/screens/screen_helper.h"
#include "ui/pages/print_control/print_control_page.h"
#include "ui/pages/printing/printing_page.h"
#include "ui/ui.h"

namespace ui {
namespace printing_screen {

void _printer_update_handler(const printer::State &state);

lv_obj_t *init(const printer::State &state) {
  lv_obj_t *scr = screen_helper::create_screen();
  control::register_printer_update_cb(scr, _printer_update_handler);

  printing_page::init(scr, state);
  print_control_page::init(scr, state);

  return scr;
}

void _printer_update_handler(const printer::State &state) {
  printing_page::printer_update(state);
  print_control_page::printer_update(state);
}

}
}