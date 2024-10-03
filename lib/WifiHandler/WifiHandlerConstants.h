#ifndef WIFI_HANDLER_CONSTANTS_H
#define WIFI_HANDLER_CONSTANTS_H

namespace WifiSettings {
    const int MAX_RETRIES = 30;
}

namespace WifiStates {
    const char* CONNECTION_MISSING_INPUT = "Wifi ssid and password required to connect";
    const char* CONNECTION_ATTEMPT_MESSAGE = "Attempting to connect to WiFi...";
    const char* CONNECTION_FAILED_MESSAGE = "Wifi connection failed with error code: ";
    const char* CONNECTION_SUCCESS_MESSAGE = "Successfully connected to the wifi";

    const char* DISCONNECTION_ATTEMPT_MESSAGE = "Already connected to WiFi, disconnecting...";
    const char* DISCONNECTION_SUCCESS_MESSAGE = "Disconnected from previous WiFi.";
}

#endif