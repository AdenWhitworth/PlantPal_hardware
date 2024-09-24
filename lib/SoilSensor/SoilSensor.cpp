#include "SoilSensor.h"
#include <Adafruit_seesaw.h>
#include <Arduino.h>
#include "SoilSensorConstants.h"

Adafruit_seesaw soilsensor;

bool initSoilSensor() {
  static bool isFirstCall = true;

  if (isFirstCall) {
    if (!soilsensor.begin(0x36)) {
      Serial.println("Couldn't find Adafruit Soil Sensor!");
      return false;
    }
    Serial.print("Seesaw Soil Sensor started! version: ");
    Serial.println(soilsensor.getVersion(), HEX);
    isFirstCall = false;
  }
  return true;
}

soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse) {
  initSoilSensor();
  currentSoilResponse = {soilsensor.getTemp(), soilsensor.touchRead(0)};
  return currentSoilResponse;
}

bool assessSoil() {
  soilSensorResponse currentSoilResponse = readSoilSensor(currentSoilResponse);
  Serial.println("Soil Temp: " + String(currentSoilResponse.soilTemperature));
  Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

  return currentSoilResponse.soilCapacitive < SoilSettings::TRIGGER_CAPACITIVE;
}
