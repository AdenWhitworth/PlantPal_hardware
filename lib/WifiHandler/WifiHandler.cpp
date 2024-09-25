#include "WifiHandler.h"
#include <WiFi.h>
#include "LedControl.h"
#include "WifiHandlerConstants.h"
#include "MqttHandler.h"
#include "LedControlConstants.h"

const char* ssid = nullptr;
const char* password = nullptr; 

void connectToWiFi() {
    if (ssid && password) {
        Serial.println("Wifi ssid and password required to connect");
        return;
    }

    Serial.println("Attempting to connect to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < WifiSettings::MAX_RETRIES) {
        delay(1000);
        Serial.print(".");
        attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        connectToMQTT();
    } else {
        Serial.println("\nFailed to connect to WiFi");
        keepBlinking = false;
        fadeToColor(ColorSettings::RED, 100);
    }
}

bool checkWifiStatus() {
    return WiFi.status() == WL_CONNECTED;
}
