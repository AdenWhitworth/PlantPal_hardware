#include <Arduino.h>
#include "SoilSensor.h"
#include "PumpControl.h"
#include "../../include/PinConfig.h"
#include "PumpControlConstants.h"
#include "SoilSensorConstants.h"
#include "MqttHandler.h"

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
  Serial.println();
  Serial.println("************************************");
  Serial.println("Correct Soil Capacitance:");

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);

  const int maxAttempts = 4;
  int attempt = 0;

  while (currentSoilResponse.soilCapacitive < SoilSettings::TARGET_CAPACITIVE && attempt < maxAttempts) {
    
    pumpWater(PumpSettings::PUMP_DURATION);
    currentSoilResponse = readSoilSensor(currentSoilResponse);
    attempt++;

    Serial.println("Pump Attempt " + String(attempt) + ": Soil Temp: " + String(currentSoilResponse.soilTemperature));
    Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

    mqttLoop();
    
    vTaskDelay(pdMS_TO_TICKS(2000));
  }

  if (attempt >= maxAttempts) {
    Serial.println("Max attempts reached. Pumping stopped.");
  } else {
    Serial.println("Target soil capacitance reached.");
  }

  Serial.println("************************************");

  logSoilSensor(currentSoilResponse,true);
}
