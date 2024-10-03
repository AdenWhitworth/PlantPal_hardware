/**
 * @file WifiHandler.cpp
 * @brief Handles WiFi connections and disconnections for the ESP32 application.
 *
 * This file contains functions for connecting to WiFi, logging the WiFi status, 
 * handling disconnections, and checking the WiFi connection status.
 */

#include <Arduino.h>
#include <WiFi.h>
#include "LedControl.h"
#include "MqttHandler.h"
#include "WifiHandler.h"
#include "LedControlConstants.h"
#include "WifiHandlerConstants.h"

String ssid = "";
String password = "";

/**
 * @brief Handles the disconnection from WiFi and MQTT.
 *
 * This function checks the MQTT connection status and disconnects from MQTT 
 * if it is connected. Then, it attempts to disconnect from WiFi, waiting 
 * for a short delay before confirming the disconnection.
 *
 * @return True if disconnection from WiFi was successful, false otherwise.
 */
bool handleDisconnections() {
    if (checkMqttStatus()) {
        disconnectFromMQTT();
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(WifiStates::DISCONNECTION_ATTEMPT_MESSAGE);
        WiFi.disconnect(); 
        vTaskDelay(pdMS_TO_TICKS(1000));
        Serial.println(WifiStates::DISCONNECTION_SUCCESS_MESSAGE);
        return true;
    }

    return false;
}

/**
 * @brief Logs the current WiFi connection status to the serial monitor.
 *
 * This function prints the SSID of the connected WiFi network and its 
 * associated IP address.
 */
void logWiFiStatus() {
    Serial.println();
    Serial.print("WiFi connected to: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

/**
 * @brief Connects to a WiFi network using the stored SSID and password.
 *
 * If the SSID or password is empty, a message is printed, and the 
 * function exits without attempting to connect. Otherwise, it attempts 
 * to connect to the specified WiFi network and logs the connection status.
 */
void connectToWiFi() {
    if (ssid.isEmpty() && password.isEmpty()) {
        Serial.println(WifiStates::CONNECTION_MISSING_INPUT);
        endBlinking(ColorSettings::ORANGE);
        return;
    }

    Serial.println(WifiStates::CONNECTION_ATTEMPT_MESSAGE);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        logWiFiStatus();
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(WifiStates::CONNECTION_SUCCESS_MESSAGE);
        connectToMQTT();
    } else {
        Serial.print(WifiStates::CONNECTION_FAILED_MESSAGE);
        Serial.println(WiFi.status());

        endBlinking(ColorSettings::ORANGE);
    }
}

/**
 * @brief Checks if the ESP32 is currently connected to WiFi.
 *
 * @return True if connected to WiFi, false otherwise.
 */
bool checkWifiStatus() {
    return WiFi.status() == WL_CONNECTED;
}