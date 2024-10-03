/**
 * @file SoilAssessment.h
 * @brief Header file for soil assessment functions in the PlantPal project.
 * 
 * This file declares functions for performing scheduled and manual 
 * soil assessments, including automatic watering assessments based 
 * on soil moisture levels.
 */

#ifndef SOILASSESSMENT_H
#define SOILASSESSMENT_H

#include "SoilSensor.h"

/**
 * @brief Performs a scheduled soil assessment.
 * 
 * This function checks the current time and performs a soil 
 * assessment at specified intervals.
 * 
 * @param currentMillis The current time in milliseconds, used to 
 *                      determine when to perform the assessment.
 */
void scheduledSoilAssessment(unsigned long currentMillis);

/**
 * @brief Performs a scheduled automatic watering assessment.
 * 
 * This function checks the soil moisture level at specified 
 * intervals and waters the soil automatically if needed.
 * 
 * @param currentMillis The current time in milliseconds, used to 
 *                      determine when to perform the assessment.
 */
void scheduledAutoWaterAssessment(unsigned long currentMillis);

/**
 * @brief Performs a manual watering assessment.
 * 
 * This function allows the user to manually initiate watering of 
 * the soil based on moisture readings.
 */
void manualWaterAssessment();

#endif
