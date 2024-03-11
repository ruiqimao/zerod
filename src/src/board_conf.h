#ifndef BOARD_CONF_H
#define BOARD_CONF_H

#define RES_H 240
#define RES_V 240

#ifdef KNOMI

#define I2C0_SPEED   100000
#define I2C0_SCL_PIN 1
#define I2C0_SDA_PIN 2

#define CST816S_ADDR    0x15
#define CST816S_IRQ_PIN 17
#define CST816S_RST_PIN 16

#define GC9A01_MOSI_PIN 14
#define GC9A01_SCLK_PIN 18
#define GC9A01_CS_PIN   20
#define GC9A01_DC_PIN   19
#define GC9A01_RST_PIN  21

#define LCD_BL_PIN      12

#endif

#endif