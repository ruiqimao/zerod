#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

namespace display {

void init();
void destroy();
void set_backlight(uint8_t target);

}

#endif