#include <Wire.h>
#include "TCA9548A.h"
#include "XGZP6847D.h"
#define SDA_PIN 17 // Must be adapted to your needs
#define SCL_PIN 18 // Must be adapted to your needs


// Creating the multiplexer object
TCA9548A TCA(0x70); // Replace 0x70 with the I2C address of your multiplexer
// Multiple multiplexers can be added in sequence!
XGZP6847D pressureSensor1(&TCA, 3, 1000); // Channel 0, pressure range up to 500 kPa (Inlet)
XGZP6847D pressureSensor2(&TCA, 4, 1000); // Channel 0, pressure range up to 500 kPa (Pump)
XGZP6847D pressureSensor3(&TCA, 5, 1000); // Channel 0, pressure range up to 500 kPa (Gun)
XGZP6847D pressureSensor4(&TCA, 6, 1000); // Channel 0, pressure range up to 500 kPa (Dry)
//XGZP6847D pressureSensor2(&TCA, 5, 260); // Channel 1, pressure range up to 260 kPa
// Add more sensors if necessary

void setup() {
  Serial.begin(115200);
  
  Wire.begin(SDA_PIN, SCL_PIN); // Initialize I2C communication, don't forget to adjust the SDA or SCL pin according to your configuration
  delay(200);
  Serial.println("Communication with TCA9548A Multiplexer & XGZP6847D Pressure Sensor");
  Serial.println("This code will query 4 pressure sensors as an example. The information is presented in sequence.");
}

void loop() {
    
      Serial.println("/////////////////////");
      float pressure1 = pressureSensor1.readPressure();
      float pressure2 = pressureSensor2.readPressure();
      float pressure3 = pressureSensor3.readPressure();
      float pressure4 = pressureSensor4.readPressure();
      Serial.print("Pressure (bar): ");
      Serial.print(pressure1);
      Serial.print(" - ");
      Serial.print(pressure2);
      Serial.print(" - ");
      Serial.print(pressure3);
      Serial.print(" - ");
      Serial.println(pressure4);
      // Reading temperature from the first sensor
      float temp1 = pressureSensor1.readTemperature();
      float temp2 = pressureSensor2.readTemperature();
      float temp3 = pressureSensor3.readTemperature();
      float temp4 = pressureSensor4.readTemperature();
      Serial.print("Temperature (°C): ");
      Serial.print(temp1);
      Serial.print(" - ");
      Serial.print(temp2);
      Serial.print(" - ");
      Serial.print(temp3);
      Serial.print(" - ");
      Serial.println(temp4);
      
      Serial.println();
      Serial.println();

      delay(500);
}