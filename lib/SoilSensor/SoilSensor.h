#include <Arduino.h>

#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

struct soilSensorResponse {
    float soilTemperature;
    uint16_t soilCapacitive;
    bool isValid;
};

bool initSoilSensor();
soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse);
bool assessSoil();
#endif
