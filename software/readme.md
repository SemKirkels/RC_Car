# Software

Firmware and supporting tools for a remote-controlled car platform. The project spans embedded STM32 firmware, ESP32 communication, gamepad interfacing, distance sensing via Time-of-Flight sensors, temperature monitoring for the motor and environment, and battery management via a BMS sensor.

## Repository structure

### `classes_template/`
Main STM32 firmware. Contains all class templates and source files for the RC car. Depends on two external ToF sensor libraries (see [External dependencies](#external-dependencies)).

### `gamepad/`
Scripts and programmes for reading gamepad input and handling communication between the gamepad, the ESP32, and the STM32.

### `website/`
Intended for use with a Raspberry Pi. Not further developed.

>This folder is not actively maintained.

## External dependencies

The firmware in `classes_template` relies on two external libraries for Time-of-Flight distance sensing:

### VL53L0X — STM32H7
HAL driver for the VL53L0X ToF sensor, ported for the STM32H7.  
<https://github.com/aminejouini/VL53L0X-STM32H7>

### VL53L1X — STM32 HAL
STM32 HAL driver for the VL53L1X long-range ToF sensor.  
<https://gitlab.com/KevinAljas/vl53l1x-stm32-hal-driver>

To compile, the libraries must be added to the project (in the root folder) and the paths must be added to the compiler via:
``` properties -> C/C++ build -> settings -> GCC compiler -> include paths ```  
``` properties -> C/C++ build -> settings -> G++ compiler -> include paths ```