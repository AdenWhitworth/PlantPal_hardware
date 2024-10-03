/**
 * @file Storage.cpp
 * @brief Implementation of storage-related functions for WiFi credentials and firmware version management.
 *
 * This file contains functions to store, load, reset, and check WiFi credentials and firmware 
 * version in the ESP32 Preferences library. The storage is managed under defined namespaces
 * for better organization.
 */

#include "Storage.h"
#include <Arduino.h>
#include <Preferences.h>
#include "../../src/utilities.h"
#include "StorageConstants.h"

Preferences preferences;

/**
 * @brief Stores the WiFi credentials (SSID and Password) in preferences.
 * 
 * This function checks if the provided SSID and Password are not empty 
 * before storing them in the ESP32 Preferences. If either is empty, 
 * an error message is printed to the Serial console.
 * 
 * @param storedSSID Reference to a String containing the WiFi SSID.
 * @param storedPassword Reference to a String containing the WiFi Password.
 */
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

/**
 * @brief Loads the WiFi credentials (SSID and Password) from preferences.
 * 
 * This function retrieves the SSID and Password from the ESP32 Preferences.
 * It checks if the keys exist before attempting to load their values, 
 * printing an error message if either key is missing.
 * 
 * @param loadedSSID Reference to a String where the loaded SSID will be stored.
 * @param loadedPassword Reference to a String where the loaded Password will be stored.
 */
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
}

/**
 * @brief Resets the WiFi preferences by clearing the stored credentials.
 * 
 * This function clears all preferences stored under the WiFi namespace,
 * effectively resetting the saved SSID and Password.
 */
void resetWifiPreferences() {
    Serial.println("Clearing stored preferences...");
    
    preferences.begin(StorageSettings::WIFI_NAMESPACE, false);
    preferences.clear();
    preferences.end();
    
    Serial.println("Preferences have been reset.");
}

/**
 * @brief Updates the firmware version stored in preferences.
 * 
 * This function clears the existing firmware version key and stores the
 * current firmware version under the system namespace in the ESP32 
 * Preferences.
 */
void updateFirmwarePreferences(){
    preferences.begin(StorageSettings::SYSTEM_NAMESPACE, false);
    preferences.clear();
    preferences.putString(StorageSettings::FIRMWARE_VERSION_KEY, StorageSettings::FIRMWARE_VERSION);
    preferences.end();
}

/**
 * @brief Checks if the stored firmware version matches the current version.
 * 
 * This function compares the stored firmware version with the current
 * firmware version. If they do not match, it resets the WiFi preferences 
 * and updates the stored firmware version.
 */
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