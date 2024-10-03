/**
 * @file PumpControl.cpp
 * @brief Control functions for managing the pump operation in the PlantPal project.
 * 
 * This file contains functions for controlling the pump, checking its status, 
 * logging soil sensor readings, and correcting soil capacitance by activating 
 * the pump based on sensor data. The pump control logic includes managing 
 * the pump's state, handling attempts to achieve target soil capacitance, 
 * and integrating with the MQTT handler for real-time monitoring.
 */
#include <Arduino.h>
#include "MqttHandler.h"
#include "PumpControl.h"
#include "SoilSensor.h"
#include "../../include/PinConfig.h"
#include "PumpControlConstants.h"
#include "SoilSensorConstants.h"

/**
 * @brief Checks if the pump is currently running.
 * 
 * This function reads the state of the pump control pin and 
 * prints the status to the Serial monitor.
 * 
 * @return true if the pump is running, false if it is off.
 */
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

/**
 * @brief Activates the pump for a specified duration.
 * 
 * This function initializes the pump control pin, turns on 
 * the pump, waits for the specified duration, and then 
 * turns off the pump. It also checks the pump status before 
 * and after activation to ensure proper operation.
 * 
 * @param pumpDuration The duration in milliseconds for which 
 *                     the pump should run.
 * @return true if the pump ran successfully and stopped as 
 *         expected, false otherwise.
 */
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

/**
 * @brief Logs the current soil sensor readings to the Serial monitor.
 * 
 * This function prints the current soil temperature and 
 * capacitance values to the Serial monitor, along with the 
 * current attempt number.
 * 
 * @param currentSoilResponse A reference to the structure 
 *                            holding soil sensor readings.
 * @param attempt A reference to the current attempt number.
 */
void logCorrectSoil(soilSensorResponse &currentSoilResponse, int &attempt){
  Serial.println("Pump Attempt " + String(attempt) + ":");
  Serial.print("Soil Temp: ");
  Serial.println(currentSoilResponse.soilTemperature);
  Serial.print("Soil Cap: ");
  Serial.println(currentSoilResponse.soilCapacitive);
}

/**
 * @brief Corrects the soil capacitance by activating the pump 
 * until the target capacitance is reached.
 * 
 * This function repeatedly activates the pump to increase the 
 * soil capacitance until the target value is achieved or the 
 * maximum number of attempts is reached. It logs each attempt 
 * and the corresponding soil sensor readings.
 */
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
