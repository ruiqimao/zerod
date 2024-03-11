#ifndef PRINTER_H
#define PRINTER_H

namespace printer {

enum class Status {
  kDisconnected = 0x00,
  kIdle         = 0x01,
  kPrinting     = 0x02,
  kShutdown     = 0x03
};

enum class TramType {
  kNone = 0x00,
  kZTA  = 0x01,
  kQGL  = 0x02
};

struct State {
  Status status = Status::kDisconnected;

  bool working = false;
  bool paused = false;

  bool homed_x = false;
  bool homed_y = false;
  bool homed_z = false;

  char _padding[3];

  int32_t hotend_temp   = 0;
  int32_t hotend_target = 0;

  int32_t bed_temp   = 0;
  int32_t bed_target = 0;

  int32_t chamber_temp   = 0;
  int32_t chamber_target = 0;

  int32_t progress = 0;

  TramType tram_type = TramType::kNone;

  char gcodes[256];
};

}

#endif