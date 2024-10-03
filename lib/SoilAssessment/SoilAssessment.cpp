/**
 * @file SoilAssessment.cpp
 * @brief Functions for scheduled and manual soil assessments in the PlantPal project.
 * 
 * This file contains functions to perform soil assessments at scheduled 
 * intervals and manually, including logging the assessments and 
 * determining whether the soil requires watering based on moisture 
 * readings.
 */
#include <Arduino.h>
#include "MqttHandler.h"
#include "SoilSensor.h"
#include "PumpControl.h"
#include "SoilAssessmentConstants.h"

unsigned long previousMillis = 0;

/**
 * @brief Logs assessment information to the serial output.
 * 
 * This function logs a contextual message related to soil assessments 
 * to the serial console for monitoring purposes.
 * 
 * @param context The context or category of the assessment being logged.
 * @param message The specific message to log for the assessment.
 */
void logAssessment(const String& context, const String& message) {
    Serial.println();
    Serial.println("*****************************");
    Serial.print("Assessment: ");
    Serial.print(context);
    Serial.print(" - Message: ");
    Serial.println(message);
    Serial.println("*****************************");
}

/**
 * @brief Performs a scheduled soil assessment.
 * 
 * This function checks the MQTT connection status and, if connected, 
 * performs a soil assessment at specified intervals, logging the 
 * results to the serial output.
 * 
 * @param currentMillis The current time in milliseconds, used to 
 *                      determine when to perform the assessment.
 */
void scheduledSoilAssessment(unsigned long currentMillis) {
    if (checkMqttStatus()){
        if (currentMillis - previousMillis >= AssessmentSettings::LOG_INTERVAL){
            previousMillis = currentMillis;

            soilSensorResponse currentSoilResponse;
            currentSoilResponse = readSoilSensor(currentSoilResponse);
            logSoilSensor(currentSoilResponse,false);
            logAssessment("scheduledSoilAssessment", "Scheduled soil assessment collected.");
        }
    }
}

/**
 * @brief Performs a scheduled automatic watering assessment.
 * 
 * This function checks the MQTT connection status and, if connected 
 * and automatic watering is enabled, assesses the soil moisture at 
 * specified intervals. If the soil needs water, it initiates watering.
 * 
 * @param currentMillis The current time in milliseconds, used to 
 *                      determine when to perform the assessment.
 */
void scheduledAutoWaterAssessment(unsigned long currentMillis){ 
    if (checkMqttStatus() && shadow_auto){
    
        if (currentMillis - previousMillis >= AssessmentSettings::AUTO_INTERVAL){
            previousMillis = currentMillis;

            bool soilNeedsWater = assessSoil();

            logAssessment("scheduledAutoWaterAssessment", "Scheduled auto assessment returns: " + soilNeedsWater ? "Soil needs water" : "Soil moisture is sufficient");
            
            if (soilNeedsWater){
                correctSoilCapacitive();
            }
        }
    }
}

/**
 * @brief Performs a manual watering assessment.
 * 
 * This function checks the MQTT connection status and, if connected 
 * and automatic watering is disabled, allows the user to manually 
 * initiate watering of the soil. Logs the assessment details.
 */
void manualWaterAssessment(){
    if (checkMqttStatus() && !shadow_auto && shadow_pump){

        if (shadow_auto) {
            logAssessment("manualWaterAssessment", "Cannot manually pump water while auto is set");
            return;
        }
        
        logAssessment("manualWaterAssessment", "User requests to pump the water.");

        correctSoilCapacitive();
        
        updateShadowReportedState();
    }
}

