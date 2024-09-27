#include <Arduino.h>
#include "SoilSensor.h"
#include "PumpControl.h"
#include "../../include/PinConfig.h"
#include "PumpControlConstants.h"
#include "SoilSensorConstants.h"

void pumpWater(int pumpDuration) {
  static bool isFirstCall = true;
  if (isFirstCall) {
    pinMode(pumpControlPin, OUTPUT);
    digitalWrite(pumpControlPin, LOW);
    isFirstCall = false;
  }

  digitalWrite(pumpControlPin, HIGH);
  vTaskDelay(pdMS_TO_TICKS(pumpDuration));
  digitalWrite(pumpControlPin, LOW);
  
}

void correctSoilCapacitive() {
  soilSensorResponse currentSoilResponse = readSoilSensor(currentSoilResponse);
  while (currentSoilResponse.soilCapacitive < SoilSettings::TARGET_CAPACITIVE && currentSoilResponse.soilCapacitive > SoilSettings::TRIGGER_CAPACITIVE) {
    pumpWater(PumpSettings::PUMP_DURATION);
    currentSoilResponse = readSoilSensor(currentSoilResponse);
  }
}
