#include <Arduino.h>
#include "WifiHandler.h"
#include <WiFi.h>
#include "LedControl.h"
#include "WifiHandlerConstants.h"
#include "MqttHandler.h"
#include "LedControlConstants.h"

String ssid = "";
String password = "";

void connectToWiFi() {
    if (ssid.isEmpty() && password.isEmpty()) {
        Serial.println("Wifi ssid and password required to connect");
        endBlinking(ColorSettings::RED);
        return;
    }

    Serial.println("Attempting to connect to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
    }
    
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect();
        Serial.println("Wifi disconnected");
        Serial.println("Try again to connect to Wifi");
        if (WiFi.waitForConnectResult() == WL_CONNECTED) {
            Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(ssid);  Serial.println("IP address: ");  Serial.println(WiFi.localIP());
        }
    } 

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        connectToMQTT();
    } else {
        Serial.println("\nFailed to connect to WiFi");

        Serial.print("WiFi connection failed with error code: ");
        Serial.println(WiFi.status());

        endBlinking(ColorSettings::RED);
    }
}

bool checkWifiStatus() {
    return WiFi.status() == WL_CONNECTED;
}
