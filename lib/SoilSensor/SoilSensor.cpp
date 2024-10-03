/**
 * @file SoilSensor.cpp
 * @brief Implementation of soil sensor functionalities.
 * 
 * This file contains functions for initializing the soil sensor, 
 * reading soil data, and assessing soil conditions using the 
 * Adafruit seesaw library.
 */

#include "SoilSensor.h"
#include <Adafruit_seesaw.h>
#include <Arduino.h>
#include "SoilSensorConstants.h"
#include <SPI.h>

Adafruit_seesaw soilsensor;

/**
 * @brief Initializes the soil sensor.
 * 
 * This function initializes the Adafruit seesaw soil sensor.
 * It checks for the sensor's presence and prints its version.
 * 
 * @return true if the sensor is initialized successfully, false otherwise.
 */
bool initSoilSensor() {
  static bool isFirstCall = true;

  if (isFirstCall) {
    if (!soilsensor.begin(0x36)) {
      Serial.println("Couldn't find Adafruit Soil Sensor!");
      isFirstCall = true;
      return false;
    }
    Serial.print("Seesaw Soil Sensor started! version: ");
    Serial.println(soilsensor.getVersion(), HEX);
    isFirstCall = false;
  }
  return true;
}

/**
 * @brief Reads the soil sensor data.
 * 
 * This function retrieves the temperature and capacitive value 
 * from the soil sensor and updates the provided soilSensorResponse 
 * structure. If initialization fails, it returns an invalid response.
 * 
 * @param currentSoilResponse A reference to the current soil sensor response.
 * @return A soilSensorResponse structure containing the temperature, 
 *         capacitive value, and validity of the reading.
 */
soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse) {
  if (!initSoilSensor()) {
    currentSoilResponse = {-999.0, 0xFFFF, false};
    Serial.println("Failed to initialize Soil Sensor.");
    return currentSoilResponse;
  }

  currentSoilResponse = {soilsensor.getTemp(), soilsensor.touchRead(0), true};
  return currentSoilResponse;
}

/**
 * @brief Assesses the soil conditions.
 * 
 * This function reads the soil data and evaluates whether the soil 
 * capacitance is below the defined trigger threshold. It logs 
 * the temperature and capacitive values.
 * 
 * @return true if the soil capacitance is below the trigger value, 
 *         false otherwise.
 */
bool assessSoil() {
  soilSensorResponse currentSoilResponse = readSoilSensor(currentSoilResponse);

  if (!currentSoilResponse.isValid) {
    Serial.println("Error reading from Soil Sensor.");
    return false;
  }

  Serial.println("Soil Temp: " + String(currentSoilResponse.soilTemperature));
  Serial.println("Soil Cap: " + String(currentSoilResponse.soilCapacitive));

  return currentSoilResponse.soilCapacitive < SoilSettings::TRIGGER_CAPACITIVE;
}