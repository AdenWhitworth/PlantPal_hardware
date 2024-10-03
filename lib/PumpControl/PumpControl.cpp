#include <Arduino.h>
#include "MqttHandler.h"
#include "PumpControl.h"
#include "SoilSensor.h"
#include "../../include/PinConfig.h"
#include "PumpControlConstants.h"
#include "SoilSensorConstants.h"

bool checkPumpRunning(){
  if(digitalRead(pumpControlPin) == HIGH){
    Serial.println("Pump is currently running");
    return true;
  }

  if (digitalRead(pumpControlPin) == LOW){
    Serial.println("Pump is currently off");
    return false;
  }

  return false;
}

bool pumpWater(int pumpDuration) {
  static bool isFirstCall = true;
  if (isFirstCall) {
    pinMode(pumpControlPin, OUTPUT);
    digitalWrite(pumpControlPin, LOW);
    isFirstCall = false;
  }

  Serial.println("Starting pump...");

  digitalWrite(pumpControlPin, HIGH);

  vTaskDelay(pdMS_TO_TICKS(pumpDuration));

  if(!checkPumpRunning()){
    Serial.println("Pump should be running but is not.");
    return false;
  }

  digitalWrite(pumpControlPin, LOW);
  
  Serial.println("Pump stopped.");

  if(!checkPumpRunning()){
    Serial.println("Pump should be off but is not.");
    return false;
  }

  return true;
}

void logCorrectSoil(soilSensorResponse &currentSoilResponse, int &attempt){
  Serial.println("Pump Attempt " + String(attempt) + ":");
  Serial.print("Soil Temp: ");
  Serial.println(currentSoilResponse.soilTemperature);
  Serial.print("Soil Cap: ");
  Serial.println(currentSoilResponse.soilCapacitive);
}

void correctSoilCapacitive() {
  Serial.println();
  Serial.println("************************************");
  Serial.println("Correct Soil Capacitance:");

  soilSensorResponse currentSoilResponse;
  currentSoilResponse = readSoilSensor(currentSoilResponse);

  int attempt = 0;
  bool successPumpWater = true;

  while (currentSoilResponse.soilCapacitive < SoilSettings::TARGET_CAPACITIVE && attempt < PumpSettings::MAX_RETRY_ATTEMPTS && successPumpWater) {
    
    successPumpWater = pumpWater(PumpSettings::PUMP_DURATION);

    currentSoilResponse = readSoilSensor(currentSoilResponse);
    attempt++;

    logCorrectSoil(currentSoilResponse, attempt);

    mqttLoop();
    
    vTaskDelay(pdMS_TO_TICKS(PumpSettings::RETRY_DELAY));
  }

  if (attempt >= PumpSettings::MAX_RETRY_ATTEMPTS) {
    Serial.println("Max attempts reached. Pumping stopped.");
  } else {
    Serial.println("Target soil capacitance reached.");
  }

  Serial.println("************************************");

  logSoilSensor(currentSoilResponse,true);
}
