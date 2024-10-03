/**
 * @file WifiHandlerConstants.h
 * @brief Header file for WiFi-related constants and settings.
 *
 * This file defines constants related to WiFi connection settings, 
 * including the maximum number of connection retries and various 
 * status messages for WiFi connection attempts and results.
 */

#ifndef WIFI_HANDLER_CONSTANTS_H
#define WIFI_HANDLER_CONSTANTS_H

namespace WifiSettings {
    const int MAX_RETRIES = 30; // Maximum number of retry attempts for connecting to WiFi.
}

namespace WifiStates {
    const char* CONNECTION_MISSING_INPUT = "Wifi ssid and password required to connect"; // Message indicating that SSID and password are required for connection.
    const char* CONNECTION_ATTEMPT_MESSAGE = "Attempting to connect to WiFi..."; // Message indicating an attempt to connect to WiFi is being made.
    const char* CONNECTION_FAILED_MESSAGE = "Wifi connection failed with error code: "; // Message indicating a failed attempt to connect to WiFi.
    const char* CONNECTION_SUCCESS_MESSAGE = "Successfully connected to the wifi"; // Message indicating successful connection to WiFi.
    const char* DISCONNECTION_ATTEMPT_MESSAGE = "Already connected to WiFi, disconnecting..."; // Message indicating an attempt to disconnect from WiFi.
    const char* DISCONNECTION_SUCCESS_MESSAGE = "Disconnected from previous WiFi."; // Message indicating successful disconnection from WiFi.
}

#endif
