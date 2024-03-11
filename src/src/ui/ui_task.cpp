#include "ui_task.h"

#include <lvgl.h>

#include "display/display.h"
#include "printer/printer.h"
#include "printer/recv/recv_state.h"
#include "ui/ui.h"
#include "ui/screens/init/init_screen.h"
#include "user_conf.h"

namespace ui {

void ui_task(void *param) {
  init();
  display::init();
  display::set_backlight(DISPLAY_BRIGHTNESS);

  while (true) {
    lv_task_handler();
    printer::recv::try_read([](const printer::State &state) {
      ui::update(state);
    });
    delay(5);
  }

  display::destroy();
}

}