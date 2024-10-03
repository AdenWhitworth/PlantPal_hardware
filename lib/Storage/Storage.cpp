#include "Storage.h"
#include <Arduino.h>
#include <Preferences.h>
#include "../../src/utilities.h"
#include "StorageConstants.h"

Preferences preferences;

void storeWifiCredentials(String &storedSSID, String &storedPassword) {
    if(storedSSID.isEmpty() || storedPassword.isEmpty()){
        Serial.println("Error: SSID or Password do not exist. No storage will occur");
        return;
    }

    preferences.begin(StorageSettings::WIFI_NAMESPACE, false);
    preferences.putString(StorageSettings::SSID_KEY, storedSSID);
    preferences.putString(StorageSettings::PASSWORD_KEY, storedPassword);
    preferences.end();
}

void loadWifiCredentials(String &loadedSSID, String &loadedPassword) {
    preferences.begin(StorageSettings::WIFI_NAMESPACE, true);
    
    bool ssidExists = preferences.isKey(StorageSettings::SSID_KEY);
    bool passwordExists = preferences.isKey(StorageSettings::PASSWORD_KEY);

    if (!ssidExists) {
        Serial.println("Error: SSID key does not exist in Preferences.");
    }
    if (!passwordExists) {
        Serial.println("Error: Password key does not exist in Preferences.");
    }

    if (ssidExists && passwordExists) {
        loadedSSID = preferences.getString(StorageSettings::SSID_KEY);
        loadedPassword = preferences.getString(StorageSettings::PASSWORD_KEY);
        Serial.println("Loaded WiFi SSID & Password.");
    }
    
    preferences.end();

    Serial.print("Loaded Wifi SSID & Password ");
}

void resetWifiPreferences() {
    Serial.println("Clearing stored preferences...");
    
    preferences.begin(StorageSettings::WIFI_NAMESPACE, false);
    preferences.clear();
    preferences.end();
    
    Serial.println("Preferences have been reset.");
}

void updateFirmwarePreferences(){
    preferences.begin(StorageSettings::SYSTEM_NAMESPACE, false);
    preferences.clear();
    preferences.putString(StorageSettings::FIRMWARE_VERSION_KEY, StorageSettings::FIRMWARE_VERSION);
    preferences.end();
}

void checkFirmware() {
    preferences.begin(StorageSettings::SYSTEM_NAMESPACE, false);
    
    if (!preferences.isKey(StorageSettings::FIRMWARE_VERSION_KEY)){
        Serial.println("Firmware is not loaded currently");
        preferences.end();
        return;
    }

    String storedVersion = preferences.getString(StorageSettings::FIRMWARE_VERSION_KEY, "");
    preferences.end();

    if (storedVersion == StorageSettings::FIRMWARE_VERSION){ 
        Serial.println("Firmware version matches. No reset needed.");
        return;
    }

    Serial.println("Firmware version mismatch. Resetting preferences...");
    
    updateFirmwarePreferences();

    resetWifiPreferences();
}
