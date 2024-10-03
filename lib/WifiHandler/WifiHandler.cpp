#include <Arduino.h>
#include <WiFi.h>
#include "LedControl.h"
#include "MqttHandler.h"
#include "WifiHandler.h"
#include "LedControlConstants.h"
#include "WifiHandlerConstants.h"

String ssid = "";
String password = "";

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

void logWiFiStatus() {
    Serial.println();
    Serial.print("WiFi connected to: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

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

bool checkWifiStatus() {
    return WiFi.status() == WL_CONNECTED;
}
