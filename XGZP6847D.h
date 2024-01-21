// XGZP6847D.h

#ifndef XGZP6847D_h
#define XGZP6847D_h

#include "Arduino.h"
#include <Wire.h>

#define XGZP6847D_SLAVE_ADDRESS 0x6D       // Adresse I2C de l'esclave
#define PRESSURE_REG 0x06   // Adresse du registre de pression
#define TEMPERATURE_REG 0x09 // Adresse du registre de température
class TCA9548A;

class XGZP6847D {
public:
    static const uint8_t SENSOR_ADDRESS = 0x6D; // fixe i2c address for XGZP6847D sensor
    XGZP6847D(TCA9548A* multiplexer, uint8_t channel, float Pressure_Range); //give the possibility to choose the right channel on multiplexer TCA9548A & the right Pressure Range. Pressure Range is in Kpa
    float readPressure(); //Return float value in bar
    float readPressurePa(); //Return float value in Pa
    float readTemperature(); //Return float value in Celsius

private:
    TCA9548A* _multiplexer; // Pointeur vers l'objet TCA9548A
    uint8_t _channel;
    void selectSensor();
    float convertPressure(long int rawData);
    float convertTemperature(long int rawData);
    float _K;
};

#endif

