#include "recv_task.h"
#include "recv_state.h"

#include <Arduino.h>

#include "printer/printer.h"

namespace printer{
namespace recv {

static const uint32_t _HEADER = 0x83ad83ad;
static const uint32_t _FOOTER = 0xf007f007;
static const int _BUF_SIZE = sizeof(printer::State) + sizeof(_FOOTER);
static char _buf[_BUF_SIZE];

static State _state;
static SemaphoreHandle_t _semaphore = nullptr;

bool _validate_footer();

void recv_task(void *param) {
  _state.status = Status::kDisconnected;
  _semaphore = xSemaphoreCreateMutex();

  uint32_t header = 0;
  while (true) {
    if (Serial.available()) {
      header = (header << 8) | Serial.read();
      if (header != _HEADER) {
        continue;
      }
      header = 0;

      size_t len = Serial.readBytes(_buf, _BUF_SIZE);
      if (len < _BUF_SIZE || !_validate_footer()) {
        write([](printer::State *state) {
          state->status = printer::Status::kDisconnected;
          strcpy(state->gcodes, "MALFORMED\nPACKET");
        });
        continue;
      }

      write([](printer::State *state) {
        memcpy((char*) state, _buf, sizeof(printer::State));
        state->status = (printer::Status) ntohl((uint32_t) state->status);
        state->hotend_temp = ntohl(state->hotend_temp);
        state->hotend_target = ntohl(state->hotend_target);
        state->bed_temp = ntohl(state->bed_temp);
        state->bed_target = ntohl(state->bed_target);
        state->chamber_temp = ntohl(state->chamber_temp);
        state->chamber_target = ntohl(state->chamber_target);
        state->progress = ntohl(state->progress);
        state->tram_type = (printer::TramType) ntohl((uint32_t) state->tram_type);
      });
    }
    delay(5);
  }
}

void try_read(std::function<void(const State&)> cb) {
  if (!_semaphore) {
    return;
  }
  if (xSemaphoreTake(_semaphore, 0) == pdTRUE) {
    cb(_state);
    xSemaphoreGive(_semaphore);
  }
}

void write(std::function<void(State*)> cb) {
  xSemaphoreTake(_semaphore, portMAX_DELAY);
  cb(&_state);
  xSemaphoreGive(_semaphore);
}

bool _validate_footer() {
  uint32_t *footer = (uint32_t*) (_buf + _BUF_SIZE - 4);
  return ntohl(*footer) == _FOOTER;
}

}
}