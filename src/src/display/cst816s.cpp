#include "cst816s.h"

#include <Arduino.h>
#include <Wire.h>

#include "board_conf.h"

namespace display {
namespace cst816s {

const uint8_t kReportRate       = 0xee;
const uint8_t kReportMode       = 0xfa;
const uint8_t kMotionMask       = 0xec;
const uint8_t kAutoReset        = 0xfb;
const uint8_t kLongReset        = 0xfc;
const uint8_t kDisableAutoSleep = 0xfe;

static bool _ready = false;

void _handle_irq();
bool _read_i2c(uint8_t reg, uint8_t *data, size_t len);
void _write_i2c(uint8_t reg, uint8_t data);

void init() {
  Wire.begin();
  pinMode(CST816S_IRQ_PIN, INPUT);
  pinMode(CST816S_RST_PIN, OUTPUT);

  digitalWrite(CST816S_RST_PIN, LOW);
  delay(10);
  digitalWrite(CST816S_RST_PIN, HIGH);
  delay(50);

  attachInterrupt(CST816S_IRQ_PIN, _handle_irq, FALLING);

  _write_i2c(kReportRate, 1);
  _write_i2c(kReportMode, 0x60);
  _write_i2c(kMotionMask, 0);
  _write_i2c(kAutoReset, 0);
  _write_i2c(kLongReset, 0);
  _write_i2c(kDisableAutoSleep, 1);
}

bool ready() {
  if (_ready) {
    _ready = false;
    return true;
  }
  return false;
}

bool read(int &x, int &y) {
  uint8_t data[6];
  if (!_read_i2c(0x01, data, 6)) {
    return false;
  }

  x = (data[2] & 0x0f << 8) | data[3];
  y = (data[4] & 0x0f << 8) | data[5];

  return data[1];
}

void _handle_irq() {
  _ready = true;
}

bool _read_i2c(uint8_t reg, uint8_t *data, size_t len) {
  Wire.beginTransmission(CST816S_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(true)) {
    return false;
  }
  uint8_t recv = Wire.requestFrom(CST816S_ADDR, static_cast<int>(len), 1);
  Wire.readBytes(data, recv);
  return true;
}

void _write_i2c(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(CST816S_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission(true);
}

}
}