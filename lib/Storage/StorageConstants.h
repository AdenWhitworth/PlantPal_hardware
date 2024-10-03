/**
 * @file StorageConstants.h
 * @brief Header file for storage-related constants used in the application.
 *
 * This file defines constants used for managing WiFi credentials and firmware version 
 * in the ESP32 Preferences.
 */
#ifndef STORAGE_CONSTANTS_H
#define STORAGE_CONSTANTS_H

/**
 * @brief Namespace for storage-related preferences.
 * 
 * This namespace contains constants used for managing 
 * WiFi credentials and firmware version in the application.
 */
namespace StorageSettings {
    const char* WIFI_NAMESPACE = "wifi"; // Namespace for WiFi-related preferences.
    const char* SYSTEM_NAMESPACE = "system"; // Namespace for system-related preferences.
    const char* SSID_KEY = "ssid"; // Key for storing the WiFi SSID.
    const char* PASSWORD_KEY = "password"; // Key for storing the WiFi Password.
    const char* FIRMWARE_VERSION_KEY = "firmwareVersion"; // Key for storing the firmware version.
    const char* FIRMWARE_VERSION = "1.0.0"; // Current firmware version of the application.
}

#endif
