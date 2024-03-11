"""Klipper extra module for ZeroD."""

import dataclasses
import enum
import serial
import struct
from typing import List


_BAUD_RATE = 115200
_HEADER = b"\x83\xad\x83\xad"
_FOOTER = b"\xf0\x07\xf0\x07"

_RECV_PERIOD = 0.1
_SEND_PERIOD = 0.1

_GCODES_MAX_LEN = 255

_CMD_PREFIX = b"ZEROD_CMD:"
_CMD_STOP = b"STOP"
_CMD_RESTART = b"RESTART"
_CMD_GCODE = b"GCODE:"
_CMD_MOVE = b"MOVE:"


class PrinterStatus(enum.Enum):
  DISCONNECTED = 0x00
  IDLE         = 0x01
  PRINTING     = 0x02
  SHUTDOWN     = 0x03


class PrinterTramType(enum.Enum):
  NONE = 0x00
  ZTA  = 0x01
  QGL  = 0x02


@dataclasses.dataclass(frozen=True)
class PrinterState:
  status: PrinterStatus = PrinterStatus.DISCONNECTED

  working: bool = False
  paused: bool = False
  homed_x: bool = False
  homed_y: bool = False
  homed_z: bool = False

  hotend_temp: float = 0
  hotend_target: float = 0

  bed_temp: float = 0
  bed_target: float = 0

  chamber_temp: float = 0
  chamber_target: float = 0

  progress: float = 0

  tram_type: PrinterTramType = PrinterTramType.NONE

  gcodes: bytes = b""


class ZeroD:

  def __init__(self, config):
    self.printer = config.get_printer()
    self.reactor = self.printer.get_reactor()
    self.name = config.get_name()

    self.gcode = None
    self.heaters = None
    self.sensors = None
    self.toolhead = None
    self.virtual_sdcard = None
    self.print_stats = None

    self.serial = None
    self.update_timer = None
    self.last_busy = 0
    self.pending_cmd = b""

    self.tram_type = PrinterTramType.NONE
    self.gcodes = b""

    self.config_serial = config.get("serial")

    self.config_hotend = config.get("heater_hotend", "extruder")
    self.config_bed = config.get("heater_bed", "heater_bed")

    self.config_heater_chamber = config.get("heater_chamber", "")
    self.config_sensor_chamber = config.get("sensor_chamber", "")
    if self.config_heater_chamber and self.config_sensor_chamber:
      raise config.error(
          "Only one of heater_chamber and sensor_chamber can be specified",
      )

    self.config_move = [
        config.getfloat("move_x", 10.0),
        config.getfloat("move_y", 10.0),
        config.getfloat("move_z", 10.0),
    ]
    self.config_speed = [
        config.getfloat("speed_x", 100.0),
        config.getfloat("speed_y", 100.0),
        config.getfloat("speed_z", 100.0),
    ]

    self.config_gcodes = config.get("gcodes", "")
    gcodes = [gcode.strip() for gcode in self.config_gcodes.split(",")]
    gcodes = [gcode for gcode in gcodes if gcode]
    gcodes = "\n".join(gcodes)
    self.gcodes = gcodes.encode("utf-8")
    if len(self.gcodes) > _GCODES_MAX_LEN:
      raise config.error(
          f"Too many G-codes specified ({len(self.gcodes)} > "
          f"{_GCODES_MAX_LEN})",
      )

    self.printer.register_event_handler("klippy:connect", self._handle_connect)
    self.printer.register_event_handler("klippy:ready", self._handle_ready)
    self.printer.register_event_handler(
        "klippy:shutdown",
        self._handle_shutdown,
    )
    self.printer.register_event_handler(
        "klippy:disconnect",
        self._handle_disconnect,
    )

  def _handle_connect(self):
    self.serial = serial.Serial(self.config_serial, _BAUD_RATE)

  def _handle_ready(self):
    self.gcode = self.printer.lookup_object("gcode")
    self.heaters = self.printer.lookup_object("heaters")
    self.toolhead = self.printer.lookup_object("toolhead")
    self.virtual_sdcard = self.printer.lookup_object("virtual_sdcard")
    self.print_stats = self.printer.lookup_object("print_stats")

    if self.printer.lookup_object("z_tilt", None):
      self.tram_type = PrinterTramType.ZTA
    elif self.printer.lookup_object("quad_gantry_level", None):
      self.tram_type = PrinterTramType.QGL
    else:
      self.tram_type = PrinterTramType.NONE

    self.update_timer = self.reactor.register_timer(
        self._handle_update,
        self.reactor.NOW,
    )
    self.read_timer = self.reactor.register_timer(
        self._handle_read,
        self.reactor.NOW,
    )

  def _handle_shutdown(self):
    self._send_shutdown_state()

  def _handle_disconnect(self):
    self._send_state(PrinterState(status=PrinterStatus.DISCONNECTED))
    if self.serial:
      self.serial.close()

  def _handle_update(self, eventtime):
    self._send_update(eventtime)
    return eventtime + _SEND_PERIOD

  def _handle_read(self, eventtime):
    if not self.serial or not self.serial.is_open:
      return

    while self.serial.in_waiting:
      char = self.serial.read()
      if char == b"\n":
        if self.pending_cmd.startswith(_CMD_PREFIX):
          self._process_cmd(self.pending_cmd[len(_CMD_PREFIX):])
        self.pending_cmd = b""
      else:
        self.pending_cmd += char

    return eventtime + _RECV_PERIOD

  def _send_update(self, eventtime):
    if self.printer.is_shutdown():
      self._send_shutdown_state()
      return

    status = PrinterStatus.IDLE
    paused = False
    if self.print_stats.state == "printing":
      status = PrinterStatus.PRINTING
    if self.print_stats.state == "paused":
      status = PrinterStatus.PRINTING
      paused = True

    _, _, lookahead_empty = self.toolhead.check_busy(eventtime)
    if not lookahead_empty:
      self.last_busy = eventtime
    working = eventtime - self.last_busy < 1

    toolhead_status = self.toolhead.get_status(eventtime)
    homed_x = "x" in toolhead_status["homed_axes"]
    homed_y = "y" in toolhead_status["homed_axes"]
    homed_z = "z" in toolhead_status["homed_axes"]

    hotend = self.heaters.lookup_heater(self.config_hotend)
    hotend_temp, hotend_target = hotend.get_temp(eventtime)

    bed = self.heaters.lookup_heater(self.config_bed)
    bed_temp, bed_target = bed.get_temp(eventtime)

    chamber_temp = 0
    chamber_target = 0
    if self.config_heater_chamber:
      chamber_heater = self.heaters.lookup_heater(self.config_heater_chamber)
      chamber_temp, chamber_target = chamber_heater.get_temp(eventtime)
    if self.config_sensor_chamber:
      chamber_sensor = self.printer.lookup_object(
          f"temperature_sensor {self.config_sensor_chamber}",
      )
      chamber_temp, _ = chamber_sensor.get_temp(eventtime)

    # Workaround for Danger Klipper.
    if hasattr(self.virtual_sdcard, "progress"):
      provider = self.virtual_sdcard
    else:
      provider = self.virtual_sdcard.get_virtual_sdcard_gcode_provider()
    progress = provider.progress() * 100

    state = PrinterState(
        status=status,
        paused=paused,
        working=working,
        homed_x=homed_x,
        homed_y=homed_y,
        homed_z=homed_z,
        hotend_temp=hotend_temp,
        hotend_target=hotend_target,
        bed_temp=bed_temp,
        bed_target=bed_target,
        chamber_temp=chamber_temp,
        chamber_target=chamber_target,
        progress=progress,
        tram_type=self.tram_type,
        gcodes=self.gcodes,
    )
    self._send_state(state)

  def _send_shutdown_state(self):
    message, _ = self.printer.get_state_message()
    message = message[:_GCODES_MAX_LEN]
    message = message.split("\n")[0]
    message = message.upper()
    message = message.encode("utf-8")
    self._send_state(PrinterState(
        status=PrinterStatus.SHUTDOWN,
        gcodes=message,
    ))

  def _send_state(self, state):
    if not self.serial or not self.serial.is_open:
      return

    msg = _HEADER
    msg += struct.pack("!I", state.status.value)
    msg += struct.pack("!?", state.working)
    msg += struct.pack("!?", state.paused)
    msg += struct.pack("!?", state.homed_x)
    msg += struct.pack("!?", state.homed_y)
    msg += struct.pack("!?", state.homed_z)
    msg += b'\x00\x00\x00'
    msg += struct.pack("!i", int(state.hotend_temp))
    msg += struct.pack("!i", int(state.hotend_target))
    msg += struct.pack("!i", int(state.bed_temp))
    msg += struct.pack("!i", int(state.bed_target))
    msg += struct.pack("!i", int(state.chamber_temp))
    msg += struct.pack("!i", int(state.chamber_target))
    msg += struct.pack("!i", int(state.progress))
    msg += struct.pack("!I", state.tram_type.value)
    msg += state.gcodes + b"\x00" * (_GCODES_MAX_LEN + 1 - len(state.gcodes))
    msg += _FOOTER

    try:
      self.serial.write(msg)
    except serial.SerialException:
      self.printer.invoke_shutdown(f"Lost connection with {self.name}")
      self.serial = None

  def _process_cmd(self, cmd):
    try:
      if cmd == _CMD_STOP:
        self.printer.invoke_shutdown(f"Stop requested by {self.name}")
        return
      if cmd == _CMD_RESTART:
        self.gcode.request_restart("firmware_restart")
        return
      if cmd.startswith(_CMD_GCODE):
        gcode = cmd[len(_CMD_GCODE):]
        self.gcode.run_script(gcode.decode("utf-8"))
        return
      if cmd.startswith(_CMD_MOVE):
        axis = {
            ord("X"): 0,
            ord("Y"): 1,
            ord("Z"): 2,
        }[cmd[-2]]
        direction = {
            ord("+"): 1,
            ord("-"): -1,
        }[cmd[-1]]
        pos = self.toolhead.get_position()
        pos[axis] += self.config_move[axis] * direction
        self.toolhead.manual_move(pos, self.config_speed[axis])
    except:
      pass


def load_config(config):
  zerod = ZeroD(config)
  return zerod
