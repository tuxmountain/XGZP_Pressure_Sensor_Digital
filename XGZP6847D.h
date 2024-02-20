//
//    FILE: XGZP6847D.h
//  AUTHOR: Cédric Rey
// VERSION: 1.1
//    DATE: 2024-02-20
// PURPOSE: ESP / Arduino library for XGZP Digital Pressure sensor
//     URL: https://github.com/tuxmountain/XGZP_Pressure_Sensor_Digital
//
#ifndef XGZP6847D_h
#define XGZP6847D_h

#include "Arduino.h"
#include <Wire.h>

#define XGZP6847D_SLAVE_ADDRESS 0x6D       // I2C address of the slave
#define PRESSURE_REG 0x06   // Pressure register address
#define TEMPERATURE_REG 0x09 // Temperature register address
class TCA9548A; // Forward declaration of the TCA9548A class

class XGZP6847D {
public:
    static const uint8_t SENSOR_ADDRESS = 0x6D; // Fixed I2C address for the XGZP6847D sensor
    XGZP6847D(TCA9548A* multiplexer, uint8_t channel, float Pressure_Range); // Constructor of the XGZP6847D class
    float readPressure(); // Function to read pressure in Bar
    float readPressurePa(); // Function to read pressure in Pa
    float readTemperature(); // Function to read temperature in degrees Celsius

private:
    TCA9548A* _multiplexer; // Pointer to the TCA9548A object for channel selection
    uint8_t _channel; // Multiplexer channel used for this sensor
    void selectSensor(); // Function to select the sensor via the multiplexer
    float _K; // Conversion factor for pressure based on the range
};


extern TwoWire MP;
#endif

