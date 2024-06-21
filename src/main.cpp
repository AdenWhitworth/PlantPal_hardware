#include <Arduino.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw soilsensor;

struct soilSensorResponse {
  float soilTemperature;
  uint16_t soilCapacitive;
};

void setup() {
  Serial.begin(115200);

  initSoilSensor();

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);
  Serial.println("Soil Temp: " + String(currentSoilResponse.soilTemperature));
  Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

}

void loop() {

}

bool initSoilSensor(){

  static bool isFirstCall = true;

  if (isFirstCall){
    if (!soilsensor.begin(0x36)) {
    Serial.println(F("Couldnt find Adafruit Soil Sensor!"));
      return false;
    }
    Serial.print("Seesaw Soil Sensor started! version: ");
    Serial.println(soilsensor.getVersion(), HEX);
    isFirstCall = false;
  }
  return true;
}

soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse) {
  
  currentSoilResponse = {
    soilsensor.getTemp(),
    soilsensor.touchRead(0)
  };

  return currentSoilResponse;

}

