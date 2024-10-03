/**
 * @file SoilSensor.h
 * @brief Header file for soil sensor functionality.
 * 
 * This file defines the data structure for soil sensor responses and 
 * declares functions for initializing the soil sensor, reading soil 
 * data, and assessing soil conditions.
 */

#include <Arduino.h>

#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

/**
 * @struct soilSensorResponse
 * @brief Structure to hold soil sensor data.
 * 
 * This structure contains the temperature and capacitive reading 
 * from the soil sensor, along with a validity flag indicating 
 * whether the readings are valid.
 */
struct soilSensorResponse {
    float soilTemperature;    // Temperature of the soil in degrees Celsius.
    uint16_t soilCapacitive;  // Capacitive reading of the soil.
    bool isValid;             // Indicates if the readings are valid.
};

/**
 * @brief Initializes the soil sensor.
 * 
 * This function sets up the soil sensor for use. It checks 
 * the sensor's presence and initializes it if found.
 * 
 * @return true if the sensor is initialized successfully, 
 *         false otherwise.
 */
bool initSoilSensor();

/**
 * @brief Reads the soil sensor data.
 * 
 * This function retrieves the temperature and capacitive 
 * value from the soil sensor and updates the provided 
 * soilSensorResponse structure. 
 * 
 * @param currentSoilResponse A reference to the current soil 
 *                            sensor response structure.
 * @return A soilSensorResponse structure containing the 
 *         temperature, capacitive value, and validity of 
 *         the reading.
 */
soilSensorResponse readSoilSensor(soilSensorResponse currentSoilResponse);

/**
 * @brief Assesses the soil conditions.
 * 
 * This function reads the soil data and evaluates whether 
 * the soil capacitance is below a defined threshold. 
 * 
 * @return true if the soil capacitance is below the 
 *         threshold, false otherwise.
 */
bool assessSoil();

#endif