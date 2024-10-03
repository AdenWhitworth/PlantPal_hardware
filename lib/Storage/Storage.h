/**
 * @file Storage.h
 * @brief Header file for storage management functions related to WiFi credentials and firmware version.
 *
 * This file declares functions for storing, loading, resetting, and checking WiFi credentials and firmware 
 * version using the ESP32 Preferences library. 
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

/**
 * @brief Stores the WiFi credentials (SSID and Password) in preferences.
 * 
 * This function saves the provided SSID and Password to the ESP32 Preferences.
 * 
 * @param storedSSID Reference to a String containing the WiFi SSID.
 * @param storedPassword Reference to a String containing the WiFi Password.
 */
void storeWifiCredentials(String &storedSSID, String &storedPassword);

/**
 * @brief Loads the WiFi credentials (SSID and Password) from preferences.
 * 
 * This function retrieves the SSID and Password from the ESP32 Preferences,
 * checking for their existence before loading.
 * 
 * @param loadedSSID Reference to a String where the loaded SSID will be stored.
 * @param loadedPassword Reference to a String where the loaded Password will be stored.
 */
void loadWifiCredentials(String &loadedSSID, String &loadedPassword);

/**
 * @brief Resets the WiFi preferences by clearing the stored credentials.
 * 
 * This function clears all stored preferences related to WiFi, including SSID 
 * and Password.
 */
void resetWifiPreferences();

/**
 * @brief Updates the firmware version stored in preferences.
 * 
 * This function saves the current firmware version in the ESP32 Preferences, 
 * clearing the previous version if it exists.
 */
void updateFirmwarePreferences();

/**
 * @brief Checks if the stored firmware version matches the current version.
 * 
 * This function compares the stored firmware version with the current version.
 * If they do not match, it resets the WiFi preferences and updates the stored 
 * firmware version.
 */
void checkFirmware();

#endif