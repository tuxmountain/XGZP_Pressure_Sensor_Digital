// XGZP6847D.cpp

#include "TCA9548A.h"
#include "XGZP6847D.h"

// Constructor of XGZP6847D class
XGZP6847D::XGZP6847D(TCA9548A* multiplexer, uint8_t channel, float Pressure_Range) {
    _multiplexer = multiplexer;
    _channel = channel;
    // Select the value of _K based on the pressure range, the pressure range is indicated in kPa
    // ... (the rest of the if-else structure remains unchanged) ...

    // Selects the correct channel on the multiplexer
}

// Function to select the appropriate sensor
void XGZP6847D::selectSensor() {
    _multiplexer->selectChannel(_channel); // Simply interconnect with the TCA9548 library to select the right channel to communicate
}

// Function to read pressure in Pascals (Pa)
float XGZP6847D::readPressurePa() {
    selectSensor(); // Selects the correct channel on the multiplexer
    Wire.beginTransmission(SENSOR_ADDRESS);
    Wire.write(0x30);  // Write 0x30 to indicate a combined conversion
    Wire.write(0x0A);  // Write 0x0A for more measurement methods (see the sensor documentation)
    Wire.endTransmission();
    
    // Wait for data collection to complete
    while (true) {
        Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)1);
        byte status = Wire.read();
        if (!status & 0x08) {
            break;
        }
    }
    // Code to start I2C transmission and read pressure data
    // (Replace PRESSURE_REG with the correct register address for your sensor)
    Wire.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    Wire.write(PRESSURE_REG); // Send command to read the pressure register
    Wire.endTransmission();

    // Request pressure data (3 bytes)
    Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)3);
    if (Wire.available() == 3) {
        uint8_t pressure_H = Wire.read(); // Read the high byte
        uint8_t pressure_M = Wire.read(); // Read the middle byte
        uint8_t pressure_L = Wire.read(); // Read the low byte

        // Convert the three bytes into a raw pressure value
        long int rawData = ((long)pressure_H << 16) | ((long)pressure_M << 8) | pressure_L;

        return (float)rawData; // Return the raw pressure in Pa
    } else {
        // Handle read error or return a specific error value
        return -999.0; // Example error value
    }
}

// Function to read pressure (converted to bar)
float XGZP6847D::readPressure() {
    float pressurePa = readPressurePa();
    if (pressurePa == -999.0) { // Check if readPressurePa() returned an error
        return pressurePa; // Return the same error
    }
    return pressurePa / 100000; // Convert Pa to bar (1 bar = 100000 Pa)
}

// Function to read temperature
float XGZP6847D::readTemperature() {
    selectSensor(); // Selects the correct channel on the multiplexer

    // Code to start I2C transmission and read temperature data
    // (Replace TEMPERATURE_REG with the correct register address for your sensor)
    Wire.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    Wire.write(TEMPERATURE_REG); // Send command to read the temperature register
    Wire.endTransmission();

    // Request temperature data (2 bytes)
    Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)2);
    if (Wire.available() == 2) {
        uint8_t temperature_H = Wire.read(); // Read the high byte
       



