# XGZP6847D Pressure Sensor Library

## Overview

The XGZP6847D is a digital pressure sensor that offers quick and precise pressure readings in air circuits. Unlike the commonly available analog versions in maker applications, this digital variant provides more accurate and temperature-calibrated readings. With a range of 0-10 bar, the XGZP6847D is versatile for a wide array of projects.

For more information, visit the manufacturer's website: [CFSensor XGZP6847D](https://cfsensor.com/product/piezoresistive-pressure-sensor-xgzp6847d/)

This library was created for easy and efficient use in your projects. Should work with any Arduino / ESP32 / STM32 project

## Key Features

- **Digital Precision**: Offers more precise readings compared to analog sensors, especially useful in temperature-variable environments.
- **Calibration**: Self-calibration.
- **Wide Range**: Capable of measuring pressures from 0 to 10 bar, suitable for diverse applications.
- **I2C Communication**: Utilizes I2C for data transfer. The sensor has a fixed address of 0x6D.
- **Multiplexer Compatibility**: Designed to work with a TCA9548A multiplexer, this library simplifies connecting multiple sensors to a project. It can manage up to 8 sensors with ease.

## Usage

1. **Initialization**: Create an object, for example, `XGZP6847D pressureSensor1(&TCA, 3, 1000);` where `&TCA` is the object for TCA9548A, `3` is the communication channel on the multiplexer and `1000` is the pressure value of your sensor in Kpa.
2. **Reading Pressure (Bar)**: `pressureSensor1.readPressure()` returns the pressure converted into bar float value.
3. **Reading Pressure (Pa)**: `pressureSensor1.readPressurePa()` returns the pressure directly in Pascal
3. **Reading Temperature**: `pressureSensor1.readTemperature()` provides the temperature in Celsius, which is useful for applications without the need for a separate temperature sensor.

## Future Development

Additional functionalities are planned to be developed and integrated into this library. Specially, negative pressure is not currently supported. Reading errors are currently displayed as -999.0. 

## Important Note

This library requires a TCA9548 multiplexer for proper operation. Each sensor must be initialized with its I2C address corresponding to the correct port on the multiplexer using the `Wire.h` library. However, connected directly to the correct I2C port, it should still work but not tested.

![image](https://github.com/tuxmountain/Technofox-IoT-System/assets/75492601/626805e5-7b01-4a4c-95f2-86d431ee1814)
