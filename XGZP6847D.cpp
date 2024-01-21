// XGZP6847D.cpp

#include "TCA9548A.h"
#include "XGZP6847D.h"


XGZP6847D::XGZP6847D(TCA9548A* multiplexer, uint8_t channel, float Pressure_Range) {
    _multiplexer = multiplexer;
    _channel = channel;
    // Sélection de la valeur de K en fonction de la plage de pression, la plage de pression est indiqué en Kpa
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
        _K = 4096; // pour 1 ≤ P ≤ 2
    }
}

void XGZP6847D::selectSensor() {
    _multiplexer->selectChannel(_channel); //Simply interconnect with the TCA9548 library to select the right channel to communicate
}

float XGZP6847D::readPressurePa() {
    selectSensor(); // Sélectionne le bon canal sur le multiplexeur
    Wire.beginTransmission(SENSOR_ADDRESS);
    Wire.write(0x30);  // Écrire 0x30 pour indiquer une conversion combinée
    Wire.write(0x0A);  // Écrire 0x0A pour plus de méthodes de mesure (consultez la documentation du capteur)
    Wire.endTransmission();
    
    //attendre la fin de la donnée
    while (true) {
    	Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)1);
    	byte status = Wire.read();
    	if (!status & 0x08) {
    		break;
    	}
    		
    // Code pour démarrer la transmission I2C et lire les données de pression
    // (Remplacez PRESSURE_REG par l'adresse de registre correcte pour votre capteur)
    Wire.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    Wire.write(PRESSURE_REG); // Envoie la commande pour lire le registre de pression
    Wire.endTransmission();

    // Demande les données de pression (3 octets)
    Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)3);
    if (Wire.available() == 3) {
        uint8_t pressure_H = Wire.read(); // Lit l'octet de poids fort
        uint8_t pressure_M = Wire.read(); // Lit l'octet de poids moyen
        uint8_t pressure_L = Wire.read(); // Lit l'octet de poids faible

        // Convertit les trois octets en une valeur brute de pression
        long int rawData = ((long)pressure_H << 16) | ((long)pressure_M << 8) | pressure_L;

        return (float)rawData; // Retourne la pression brute en Pa
    } else {
        // Gère l'erreur de lecture ou retourne une valeur d'erreur spécifique
        return -999.0; // Exemple de valeur d'erreur
    }
}
}

float XGZP6847D::readPressure() {
    float pressurePa = readPressurePa();
    if (pressurePa == -999.0) { // Vérifie si readPressurePa() a retourné une erreur
        return pressurePa; // Retourne la même erreur
    }
    return pressurePa / 100000; // Convertit Pa en Bar (1 Bar = 100000 Pa)
}

float XGZP6847D::readTemperature() {
    selectSensor(); // Sélectionne le bon canal sur le multiplexeur

    // Code pour démarrer la transmission I2C et lire les données de température
    // (Remplacez TEMPERATURE_REG par l'adresse de registre correcte pour votre capteur)
    Wire.beginTransmission(XGZP6847D::SENSOR_ADDRESS);
    Wire.write(TEMPERATURE_REG); // Envoie la commande pour lire le registre de température
    Wire.endTransmission();

    // Demande les données de température (2 octets)
    Wire.requestFrom((uint8_t)XGZP6847D::SENSOR_ADDRESS, (uint8_t)2);
    if (Wire.available() == 2) {
        uint8_t temperature_H = Wire.read(); // Lit l'octet de poids fort
        uint8_t temperature_L = Wire.read(); // Lit l'octet de poids faible

        // Convertit les deux octets en une valeur brute de température
        long int rawData = ((long)temperature_H << 8) | temperature_L;

        // Implémente ici la logique de conversion des données brutes en degrés Celsius
        // La formule exacte dépendra des spécifications de votre capteur
        return convertTemperature(rawData); // Convertit et retourne la température en °C
    } else {
        // Gère l'erreur de lecture ou retourne une valeur d'erreur spécifique
        return -999.0; // Exemple de valeur d'erreur
    }
}


float XGZP6847D::convertPressure(long int rawData) {
    if (rawData > 8388608) { // Vérifie si la pression est négative
        return (rawData - 16777216) / _K; // Conversion pour pression négative
    } else {
        return rawData / _K; // Conversion pour pression positive
    }
}

float XGZP6847D::convertTemperature(long int rawData) {
    float temperature;
    if (rawData > 32767) {
        temperature = (rawData - 65536) / 256.0;
    } else {
        temperature = rawData / 256.0;
    }
    return temperature;
}



