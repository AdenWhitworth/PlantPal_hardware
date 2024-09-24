#include <Arduino.h>
#include "SoilSensor.h"
#include "PumpControl.h"
#include "PinConfig.h"
#include "PumpControlConstants.h"
#include "SoilSensorConstants.h"

void pumpWater(int maxPumpSpeed, int rampDelay, int pumpDuration) {
  static bool isFirstCall = true;
  if (isFirstCall) {
    pinMode(pumpControlPin, OUTPUT);
    isFirstCall = false;
  }

  for (int pumpSpeed = 0; pumpSpeed <= maxPumpSpeed; pumpSpeed++) {
    analogWrite(pumpControlPin, pumpSpeed);
    delay(rampDelay);
  }

  delay(pumpDuration);

  for (int pumpSpeed = maxPumpSpeed; pumpSpeed >= 0; pumpSpeed--) {
    analogWrite(pumpControlPin, pumpSpeed);
    delay(rampDelay);
  }
}

void correctSoilCapacitive() {
  soilSensorResponse currentSoilResponse = readSoilSensor(currentSoilResponse);
  while (currentSoilResponse.soilCapacitive < SoilSettings::TARGET_CAPACITIVE && currentSoilResponse.soilCapacitive > SoilSettings::TRIGGER_CAPACITIVE) {
    pumpWater(PumpSettings::MAX_PUMP_SPEED, PumpSettings::RAMP_DELAY, PumpSettings::PUMP_DURATION);
    currentSoilResponse = readSoilSensor(currentSoilResponse);
  }
}
