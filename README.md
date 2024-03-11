# ZeroD

## Zero-shaped display for zero-sized printers

ZeroD is an alternative firmware for the BTT Knomi V2 and other similar displays that replaces the
network-reliant Moonraker connection with a direct serial connection to the Klipper host.

### Installation

ZeroD firmware can be built and flahsed using [PlatformIO](https://platformio.org/).

An additional Klipper module is needed to enable the ZeroD configuration options, which can be
installed by running the included `install.sh` script on the Klipper host.

### Klipper configuration
```
[zerod]
serial:  # Path to the serial port for the ZeroD device.

heater_hotend: extruder  # Name of the hotend heater.
heater_bed: heater_bed   # Name of the bed heater.

# Only one of heater_chamber and sensor_chamber should be configured.
heater_chamber:  # Name of the chamber heater.
sensor_chamber:  # Name of the chamber thermistor.

move_x: 10  # Number of millimeters to move the toolhead in the positive X direction.
move_y: 10  # Number of millimeters to move the toolhead in the positive Y direction.
move_z: 10  # Number of millimeters to move the toolhead in the positive Z direction.

speed_x: 100  # Speed to move the toolhead in the X direction.
speed_y: 100  # Speed to move the toolhead in the Y direction.
speed_z: 100  # Speed to move the toolhead in the Z direction.

gcodes:  # Comma separated G-Codes to display on the ZeroD device.
```
