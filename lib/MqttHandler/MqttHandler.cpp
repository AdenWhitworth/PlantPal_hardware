#include <Arduino.h>
#include "MQTTHandler.h"
#include <MQTT.h>
#include <WiFiClientSecure.h>
#include "../../src/utilities.h"
#include <ArduinoJson.h>
#include "LedControl.h"
#include "SoilSensor.h"
#include "WifiHandler.h"
#include "LedControlConstants.h"

WiFiClientSecure net;
MQTTClient client(1024);

bool shadow_auto;
bool shadow_pump;
bool checkMQTTShadow = false; 
bool checkInterupt = false;

void getAndCheckShadowState() {
  Serial.println("Retrieving initial shadow state...");
  
  if (client.publish(AWS_IOT_SHADOW_GET, "{}")) {
    Serial.println("Shadow get request published");
  } else {
    Serial.println("Failed to publish shadow get request");
  }
}

void updateShadowReportedState() {
  StaticJsonDocument<256> doc;
  JsonObject state = doc.createNestedObject("state");
  JsonObject reported = state.createNestedObject("reported");
  reported["auto"] = shadow_auto;
  reported["pump"] = shadow_pump;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_SHADOW_UPDATE, jsonBuffer);

  Serial.println("sent:");
  Serial.print("- topic: ");
  Serial.println(AWS_IOT_SHADOW_UPDATE);
  Serial.print("- payload:");
  Serial.println(jsonBuffer);
}

void logSoilSensor(soilSensorResponse currentSoilResponse, bool didWater) {
  StaticJsonDocument<256> doc;
  doc["cat_num"] = catNum;
  doc["soil_temp"] = currentSoilResponse.soilTemperature;
  doc["soil_cap"] = currentSoilResponse.soilCapacitive;
  doc["water"] = didWater;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);

  Serial.println("sent:");
  Serial.print("- topic: ");
  Serial.println(AWS_IOT_PUBLISH_TOPIC);
  Serial.print("- payload:");
  Serial.println(jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("received:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  StaticJsonDocument<1024> doc;
  deserializeJson(doc, payload);

  if (topic == AWS_IOT_SHADOW_UPDATE_DELTA){
    JsonObject shadow_state = doc["state"];

    if (shadow_state.containsKey("auto")){
      shadow_auto = shadow_state["auto"];
    }

    if (shadow_state.containsKey("pump")){
      shadow_pump = shadow_state["pump"];
    }

    Serial.println("shadow_auto: " + String(shadow_auto));
    Serial.println("shadow_pump: " + String(shadow_pump));
    updateShadowReportedState();
    return;
  }

  if (topic == AWS_IOT_SHADOW_ACCEPTED){
    JsonObject shadow_state = doc["state"];
    JsonObject shadow_desired = shadow_state["desired"];
    JsonObject shadow_reported = shadow_state["reported"];
  
    if (shadow_desired.containsKey("auto") && shadow_desired["auto"] != shadow_reported["auto"]) {
      shadow_auto = shadow_desired["auto"];
    }

    if (shadow_desired.containsKey("pump") && shadow_desired["pump"] != shadow_reported["pump"]) {
      shadow_pump = shadow_desired["pump"];
    }

    updateShadowReportedState();
    return;
  }

  if (topic == AWS_IOT_SHADOW_REJECTED){
    Serial.println("Failed to get shadow document");
    return;
  }

  if (topic == AWS_IOT_SUBSCRIBE_TOPIC){
    if (doc["error"]) {
      Serial.println("Failed to log sensor data");
    } else {
      Serial.println("Success logging sensor data");
    }
  }


}

void connectToMQTT() {

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  client.onMessage(messageHandler);

  Serial.print("ESP32 - Connecting to MQTT broker");

    while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!checkMqttStatus()) {
    Serial.println("ESP32 - AWS IoT Timeout!");
    keepBlinking = false;
    fadeToColor(ColorSettings::RED, 100); 
    return;
  }

  client.subscribe(AWS_IOT_SHADOW_ACCEPTED);
  client.subscribe(AWS_IOT_SHADOW_REJECTED);
  client.subscribe(AWS_IOT_SHADOW_UPDATE_DELTA);
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("ESP32  - AWS IoT Connected!");
  keepBlinking = false;
  fadeToColor(ColorSettings::GREEN, 100);
  checkMQTTShadow = true; 
}

void mqttLoop() {
  client.loop();
}

bool checkMqttStatus() {
  return client.connected();
}

void retrieveShadowOnMqttConnection(){
  if (checkWifiStatus && checkMQTTShadow) {
    getAndCheckShadowState();
    checkMQTTShadow = false;
  } 
}