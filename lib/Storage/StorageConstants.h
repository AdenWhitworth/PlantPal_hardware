/**
 * @file StorageConstants.h
 * @brief Header file for storage-related constants used in the application.
 *
 * This file defines constants used for managing WiFi credentials and firmware version 
 * in the ESP32 Preferences.
 */

#ifndef STORAGE_CONSTANTS_H
#define STORAGE_CONSTANTS_H

namespace StorageSettings {
    /**
     * @brief Namespace for WiFi-related preferences.
     */
    const char* WIFI_NAMESPACE = "wifi";
    
    /**
     * @brief Namespace for system-related preferences.
     */
    const char* SYSTEM_NAMESPACE = "system";
    
    /**
     * @brief Key for storing the WiFi SSID.
     */
    const char* SSID_KEY = "ssid";
    
    /**
     * @brief Key for storing the WiFi Password.
     */
    const char* PASSWORD_KEY = "password";
    
    /**
     * @brief Key for storing the firmware version.
     */
    const char* FIRMWARE_VERSION_KEY = "firmwareVersion";
    
    /**
     * @brief Current firmware version of the application.
     */
    const char* FIRMWARE_VERSION = "1.0.0"; 
}

#endif