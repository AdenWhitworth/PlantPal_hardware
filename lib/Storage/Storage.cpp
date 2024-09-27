#include "Storage.h"
#include <Arduino.h>
#include <Preferences.h>
#include "../../src/utilities.h"

Preferences preferences;

const char* firmwareVersion = "1.0.1"; 

void storeWifiCredentials(String &storedSSID, String &storedPassword) {
    if(storedSSID.isEmpty() || storedPassword.isEmpty()){
        Serial.println("Error: SSID or Password do not exist. No storage will occur");
        return;
    }

    preferences.begin("wifi", false);
    preferences.putString("ssid", storedSSID);
    preferences.putString("password", storedPassword);
    preferences.end();
}

void loadWifiCredentials(String &loadedSSID, String &loadedPassword) {
    preferences.begin("wifi", true);
    
    if (!preferences.isKey("ssid") && !preferences.isKey("password")){
        Serial.println("Error: SSID or Password keys do not exist in Preferences.");
        preferences.end();
        return;
    }

    loadedSSID = preferences.getString("ssid");
    loadedPassword = preferences.getString("password");
    preferences.end();

    Serial.print("Loaded Wifi SSID & Password ");
}

void resetWifiPreferences() {
    Serial.println("Clearing stored preferences...");
    
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
    
    Serial.println("Preferences have been reset.");
}

void updateFirmwarePreferences(){
    preferences.begin("system", false);
    preferences.clear();
    preferences.putString("firmwareVersion", firmwareVersion);
    preferences.end();
}

void checkFirmware() {
    preferences.begin("system", false);
    
    if (!preferences.isKey("firmwareVersion")){
        Serial.println("Firmware is not loaded currently");
        preferences.end();
        return;
    }

    String storedVersion = preferences.getString("firmwareVersion", "");
    preferences.end();

    if (storedVersion == firmwareVersion){ 
        Serial.println("Firmware version matches. No reset needed.");
        return;
    }

    Serial.println("Firmware version mismatch. Resetting preferences...");
    
    updateFirmwarePreferences();

    resetWifiPreferences();
}
