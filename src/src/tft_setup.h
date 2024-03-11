#ifndef TFT_SETUP_H
#define TFT_SETUP_H

#include "board_conf.h"

#define USER_SETUP_LOADED

#define DISABLE_ALL_LIBRARY_WARNINGS

#define USER_SETUP_ID 46

#define GC9A01_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

#define TFT_MOSI GC9A01_MOSI_PIN
#define TFT_SCLK GC9A01_SCLK_PIN
#define TFT_CS   GC9A01_CS_PIN
#define TFT_DC   GC9A01_DC_PIN
#define TFT_RST  GC9A01_RST_PIN
#define TFT_INVERSION_ON

#define SPI_FREQUENCY  80000000
#define SPI_READ_FREQUENCY  5000000

#endif