#include <Arduino.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw soilsensor;

/* ESP32 PINS */
int pumpControl = 9;

/* Global Variables */
int maxSoilCapacitive = 2000; //very wet soil
int minSoilCapacitive = 200; // very dry soil
int targetSoilCapacitive = 1200; //watered to this point
int triggerSoilCapacitive = 600; //begin watering at this point

struct soilSensorResponse {
  float soilTemperature;
  uint16_t soilCapacitive;
};

void setup() {
  Serial.begin(115200);

  initSoilSensor();

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

bool assessSoil(){

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);
  Serial.println("Soil Temp: " + String(currentSoilResponse.soilTemperature));
  Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

  if (int(currentSoilResponse.soilCapacitive) < triggerSoilCapacitive){
    return true;
  } else {
    return false;
  }

}

/* Pump Functions */

void pumpWater(int maxPumpSpeed, int rampDelay, int pumpDuration){

  static bool isFirstCall = true;

  if (isFirstCall){
    pinMode(pumpControl, OUTPUT);
    isFirstCall = false;
  }

  if ( maxPumpSpeed > 255){
    return;
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

void correctSoilCapacitive(){

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);

  while (int(currentSoilResponse.soilCapacitive) < targetSoilCapacitive && int(currentSoilResponse.soilCapacitive) > triggerSoilCapacitive){

    pumpWater(80,5,1000);

    currentSoilResponse = readSoilSensor(currentSoilResponse);

  }

}

