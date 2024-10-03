#include <Arduino.h>
#include "MqttHandler.h"
#include "SoilSensor.h"
#include "PumpControl.h"
#include "SoilAssessmentConstants.h"

unsigned long previousMillis = 0;

void logAssessment(const String& context, const String& message) {
    Serial.println();
    Serial.println("*****************************");
    Serial.print("Assessment: ");
    Serial.print(context);
    Serial.print(" - Message: ");
    Serial.println(message);
    Serial.println("*****************************");
}

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

