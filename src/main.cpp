#include <Arduino.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw soilsensor;

/* ESP32 PINS */
int pumpControl = 9;

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

/* Init Functions */

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

/* Sensor Functions */

soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse) {
  
  currentSoilResponse = {
    soilsensor.getTemp(),
    soilsensor.touchRead(0)
  };

  return currentSoilResponse;

}

/* Pump Functions */

bool pumpWater(int maxPumpSpeed, int rampDelay, int pumpDuration){

  static bool isFirstCall = true;

  if (isFirstCall){
    pinMode(pumpControl, OUTPUT);
    isFirstCall = false;
  }

  if ( maxPumpSpeed > 255){
    return false;
  }
  
  //ramp up pump
  for(int pumpSpeed = 0; pumpSpeed <= maxPumpSpeed; pumpSpeed++){
    analogWrite(pumpControl, pumpSpeed);
    delay(rampDelay);
  }

  delay(pumpDuration);

  //ramp down pump
  for(int pumpSpeed = 0; pumpSpeed >= 0; pumpSpeed--){
    analogWrite(pumpControl, pumpSpeed);
    delay(rampDelay);
  }   

}

