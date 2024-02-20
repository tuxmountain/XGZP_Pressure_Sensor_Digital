//    FILE: XGZP6847D.cpp
//  AUTHOR: Cédric Rey
// VERSION: 1.1
//    DATE: 2024-02-20
// PURPOSE: ESP / Arduino library for XGZP Digital Pressure sensor
//     URL: https://github.com/tuxmountain/XGZP_Pressure_Sensor_Digital

//#######################################
// LIBRARIES
//Libraries are include here.
#include "XGZP6847D.h"
#include "TCA9548A.h"

#define SDA_PIN 7
#define SCL_PIN 8
#define MP_SDA_PIN 16
#define MP_SCL_PIN 17
//#######################################
//OBJECTS
//Object definition, is apparently necessary to create object in the main firmware (.ino), I wasn't able to add directly this in the header of Technofox unfortenatly*/
TwoWire I2C = TwoWire(0);                // Create an object "I2C" to use function of Two.Wire to allow working on 2 different I2C Bus on my PCB
TwoWire MP = TwoWire(1);                 // Create an object "MP" to use function of Two.Wire to allow working on 2 different I2C Bus on my PCB, MP is the reference of Multiplexer used on the second Bus to interconnect slave devices with the same I2C address

// Now is necessary to create first the &MP object to connect with other slave device place after muxing.
// Multiple devices can be added in sequence, up to 8 connected to Muliplexer channel
TCA9548A TCA(0x70, &MP);  // Create an object TCA on &MP bus

//Pressure sensor use on TCA
XGZP6847D PS1(&TCA, 1, 1000);   // Channel 1, pressure range up to 1000 kPa
XGZP6847D PS2(&TCA, 2, 1000);   // Channel 1, pressure range up to 1000 kPa
//XGZP6847D PS3(&TCA, 3, 1000);   // Channel 1, pressure range up to 1000 kPa
//XGZP6847D PS4(&TCA, 4, 1000);   // Channel 1, pressure range up to 1000 kPa
//XGZP6847D PS5(&TCA, 5, 1000);   // Channel 1, pressure range up to 1000 kPa
//... you can set up to 8 Sensors


//#######################################
// INITIALISATION OF THE BOARD
// From this part of the firmware, we can decide to activate / desactivate the use of different module / function on the PCB
void setup() {
  Serial.begin(115200);
  I2C.begin(SDA_PIN, SCL_PIN, 100000);
  MP.begin(MP_SDA_PIN, MP_SCL_PIN, 100000);
  Serial.print("///////////////////////////////////////////////////////////////");
  Serial.print(" Digital Pressure Sensor XGZP6847D Library General Usage ");
  Serial.println("/////////////////////////////////////////////////////////////");

}
void loop() {
      float PS1_Bar = PS1.readPressure();
      float PS2_Bar = PS2.readPressure();
      float PS1_Temp = PS1.readTemperature();
      float PS2_Temp = PS2.readTemperature();
      float PS1_Pa = PS1.readPressurePa();
      float PS2_Pa = PS2.readPressurePa();
      
      Serial.print("Sensor 1 : "); Serial.print(PS1_Bar); Serial.print(" Bar / "); Serial.print(PS1_Pa); Serial.print("  Temperature: "); Serial.print(PS1_Temp); Serial.println("Celsius");
      Serial.print("Sensor 2 : "); Serial.print(PS2_Bar); Serial.print(" Bar / "); Serial.print(PS2_Pa); Serial.print("  Temperature: "); Serial.print(PS2_Temp); Serial.println("Celsius");
      // you can continue with other... it's just a small demo!
      delay(500);
}

