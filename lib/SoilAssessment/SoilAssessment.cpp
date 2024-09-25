#include "MqttHandler.h"
#include "SoilSensor.h"
#include "PumpControl.h"
#include <Arduino.h>

unsigned long previousMillis = 0;
const unsigned long logInterval = 4 * 60 * 60 * 1000; // 4 hours in milliseconds
const unsigned long autoInterval = 60 * 1000; // 1 minute in milliseconds

void scheduledSoilAssessment(unsigned long currentMillis) {
    
    if (!checkMqttStatus()){
        return;
    }
    
    if (currentMillis - previousMillis >= logInterval){
        previousMillis = currentMillis;

        soilSensorResponse currentSoilResponse;
        currentSoilResponse = readSoilSensor(currentSoilResponse);

        logSoilSensor(currentSoilResponse,false);

        Serial.println("Soil assessment published");
    }
}

void scheduledAutoWaterAssessment(unsigned long currentMillis){ 
    if (!checkMqttStatus()){
        return;
    }

    if (!shadow_auto){
        Serial.println("Auto watering is not selected.");
        return;
    }
    
    if (currentMillis - previousMillis >= autoInterval){
        previousMillis = currentMillis;

        bool soilNeedsWater = assessSoil();

        Serial.println(soilNeedsWater ? "Soil needs water" : "Soil moisture is sufficient");
        
        if (soilNeedsWater){
            correctSoilCapacitive();

            soilSensorResponse currentSoilResponse;
            currentSoilResponse = readSoilSensor(currentSoilResponse);

            logSoilSensor(currentSoilResponse,true);

            Serial.println("Soil auto watering published");
        }
    }
}

void manualWaterAssessment(){
    if (!checkMqttStatus()){
        return;
    }

    if (!shadow_pump){
        Serial.println("Manual watering is not selected.");
        return;
    }

    if (shadow_auto){
        Serial.println("Cannot manual water when auto water is set");
        return;
    }
    
    Serial.println("User requests to pump water");

    correctSoilCapacitive();
      
    soilSensorResponse currentSoilResponse;
    currentSoilResponse = readSoilSensor(currentSoilResponse);

    logSoilSensor(currentSoilResponse,true);
    shadow_pump = false;
    updateShadowReportedState();
    
    Serial.println("Soil manual watering published");
}

