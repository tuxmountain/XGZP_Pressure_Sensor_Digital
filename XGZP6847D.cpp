//
//    FILE: XGZP6847D.cpp
//  AUTHOR: Cédric Rey
// VERSION: 1.1
//    DATE: 2024-02-20
// PURPOSE: ESP / Arduino library for XGZP Digital Pressure sensor
//     URL: https://github.com/tuxmountain/XGZP_Pressure_Sensor_Digital
//

#include "TCA9548A.h"
#include "XGZP6847D.h"

// Constructor of the XGZP6847D class
XGZP6847D::XGZP6847D(TCA9548A* multiplexer, uint8_t channel, float Pressure_Range) {
    _multiplexer = multiplexer; // Initialize the pointer to the multiplexer
    _channel = channel; // Initialize the multiplexer channel for this sensor
    // Select the value of _K based on the pressure range (in kPa)
    if (Pressure_Range > 500 && Pressure_Range <= 1000) {
        _K = 8;
    } else if (Pressure_Range > 260) {
        _K = 16;
    } else if (Pressure_Range > 130) {
        _K = 32;
    } else if (Pressure_Range > 65) {
        _K = 64;
    } else if (Pressure_Range > 32) {
        _K = 128;
    } else if (Pressure_Range > 16) {
        _K = 256;
    } else if (Pressure_Range > 8) {
        _K = 512;
    } else if (Pressure_Range > 4) {
        _K = 1024;
    } else if (Pressure_Range > 2) {
        _K = 2048;
    } else {
        _K = 4096; // For 1 ≤ P ≤ 2
    }
}

// Function to select the sensor via the multiplexer
void XGZP6847D::selectSensor() {
    _multiplexer->selectChannel(_channel); // Use the multiplexer to select the correct channel
}

// Function to read pressure in Pa
float XGZP6847D::readPressurePa() {
    int pressure;
    selectSensor(); // Select the correct channel on the multiplexer
    MP.beginTransmission(SENSOR_ADDRESS); // Start I2C communication with the sensor
    MP.write(0x30);  // Send 0x30 to indicate combined conversion
    MP.write(0x0A);  // Send 0x0A for more measurement methods (refer to sensor documentation)
    MP.endTransmission(); // End I2C transmission

    // Wait for conversion to finish
    while (true) {
        MP.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)1);
        byte status = MP.read();
        if ((status & 0x08) == 0) {
            break;
        }
    }

    // Start I2C transmission to read pressure data (Replace PRESSURE_REG with correct register address for your sensor)
    MP.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    MP.write(PRESSURE_REG); // Send command to read pressure register
    MP.endTransmission(false); // Do not end I2C communication to allow consecutive reading

    // Request pressure data (3 bytes)
    MP.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)3);
    if (MP.available() == 3) {
        uint8_t pressure_H = MP.read(); // Read high byte
        uint8_t pressure_M = MP.read(); // Read middle byte
        uint8_t pressure_L = MP.read(); // Read low byte

        // Convert three bytes to raw pressure value
        long int rawData = pressure_H * 65536 + pressure_M * 256 + pressure_L;

        if (rawData & 0x800000) { // Pressure value is negative
            pressure = (rawData - (1L << 24)) / _K; // Apply conversion formula for negative pressure
        } else {
            pressure = rawData / _K; // Apply conversion formula for positive pressure
        }

        return (float)pressure; // Return raw pressure in Pa
    } else {
        // Handle read error or return specific error value
        return -999.0; // Example error value
    }
}

// Function to read pressure in Bar
float XGZP6847D::readPressure() {
    float pressurePa = readPressurePa();
    if (pressurePa == -999.0) { // Check if readPressurePa() returned an error
        return pressurePa; // Return the same error
    }
    return pressurePa / 100000; // Convert Pa to Bar (1 Bar = 100000 Pa)
}

// Function to read temperature in degrees Celsius
float XGZP6847D::readTemperature() {
    selectSensor(); // Select the correct channel on the multiplexer

    // Start I2C transmission to read temperature data (Replace TEMPERATURE_REG with correct register address for your sensor)
    MP.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    MP.write(TEMPERATURE_REG); // Send command to read temperature register
    MP.endTransmission();

    // Request temperature data (2 bytes)
    MP.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)2);
    if (MP.available() == 2) {
        uint8_t temperature_H = MP.read(); // Read high byte
        uint8_t temperature_L = MP.read(); // Read low byte

        // Convert two bytes to raw temperature value
        long int rawData = ((long)temperature_H << 8) | temperature_L;

        // Implement here the logic to convert raw data to degrees Celsius
        float temperature;
        if (rawData > 32767) {
            temperature = (rawData - 65536) / 256.0;
        } else {
            temperature = rawData / 256.0;
        }
        return temperature;
    } else {
        // Handle read error or return specific error value
        return -999.0; // Example error value
    }
}



